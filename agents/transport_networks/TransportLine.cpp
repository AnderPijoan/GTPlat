#include "TransportLine.h"

#include "agents/vehicles/Vehicle.h"
#include "environment/Environment.h"
#include "skills/store/AgentStoreSkill.h"

#include "agents/vehicles/Bicycle.h"
#include "agents/vehicles/Motorcycle.h"
#include "agents/people/Person.h"
#include "utils/geometry/GSSPoint.h"

TransportLine::TransportLine(TransportNode* from, TransportNode* to, GSSLineString* geom , QObject *parent) : Agent( geom , parent ) , GSSGraphEdge( from , to ) {
    this->max_speed = SpeedUnit( 1 );
    this->crossing_price = CurrencyUnit( 0 );
    this->temporarily_closed = this->permanently_closed = false;

    this->addSkill( new AgentStoreSkill(this) );
}

TransportLine::~TransportLine(){
}

QJsonObject TransportLine::toJSON(){
    QJsonObject agent_json = Agent::toJSON();
    agent_json.insert( "maximum-speed", CustomJsonExporter::format( this->max_speed ) );
    agent_json.insert( "length", CustomJsonExporter::format( this->getLength() ) );
    agent_json.insert( "gradient", CustomJsonExporter::format( this->getGradient() ) );
    agent_json.insert( "temporarily-closed", CustomJsonExporter::format( this->temporarily_closed ) );
    agent_json.insert( "permanently-closed", CustomJsonExporter::format( this->permanently_closed ) );
    return agent_json;
}

void TransportLine::fromJSON(QJsonObject json){
    Agent::fromJSON(json);
    if( json.contains( "properties" ) ){
        /*QJsonObject properties = json["properties"].toObject();
        if( properties.contains("max_speed") ){ this->max_speed = properties["max_speed"].toDouble( 0.1 ); }
        if( properties.contains("length") ){ this->setLength( properties["length"].toDouble() ); }
        if( properties.contains("temporarily_closed") ){ this->temporarily_closed = properties["temporarily_closed"].toBool( false ); }
        if( properties.contains("permanently_closed") ){ this->permanently_closed = properties["permanently_closed"].toBool( false ); }*/
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

TransportNode* TransportLine::getFromNode() const{
    return dynamic_cast<TransportNode*>( GSSGraphEdge::getFromNode() );
}

TransportNode* TransportLine::getToNode() const{
    return dynamic_cast<TransportNode*>( GSSGraphEdge::getToNode() );
}

const QStringList TransportLine::getAllowedAgents() const{
    return this->allowed_agent_classes;
}

bool TransportLine::isAllowed(QString class_name) const{
    return this->allowed_agent_classes.contains( class_name );
}

bool TransportLine::isAllowed(Agent *agent) const{
    const QMetaObject* obj = agent->metaObject();
    while( obj ){
        if( this->isAllowed( obj->className() ) ){
            return true;
        }
        obj = obj->superClass();
    }
    return false;
}

/**
 * @brief Road::getGradient Get the roads gradient
 * (positive for climbing up and negative for going down)
 * @return
 */
double TransportLine::getGradient() const{
    if( this->getLength().number() >= 0 ){
        try {

            double height ( this->getFromNode()->getCoordinate().getZ() - this->getToNode()->getCoordinate().getZ() );
            if( height == height ){ // Avoid NaN
                return ( height - 100 / this->getLength().number() );
            }
        } catch(...){}
    }
    return 0;
}

SpeedUnit TransportLine::getMaxSpeed() const{
    return this->max_speed;
}

CurrencyUnit TransportLine::getCrossingPrice() const{
    return this->crossing_price;
}

bool TransportLine::isTemporarilyClosed() const{
    return this->temporarily_closed;
}

bool TransportLine::isPermanentlyClosed() const{
    return this->permanently_closed;
}

bool TransportLine::isAgentInside(Agent *agent) const{
    AgentStoreSkill* c = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className()) );
    return c->isAgentInside( agent );
}

bool TransportLine::isFull() const{
    AgentStoreSkill* c = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className()) );
    return c->isFull();
}

double TransportLine::getCost() const{
    return this->getLength().number() / this->getMaxSpeed().number();
}

unsigned int TransportLine::getMaxCapacity() const{
    AgentStoreSkill* c = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className()) );
    return c->getMaxCapacity();
}

QList<Agent*> TransportLine::getAgentsInside() const{
    AgentStoreSkill* c = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className()) );
    return c->getAgentsInsideByClass( Agent::staticMetaObject.className() );
}

/**********************************************************************
 SETTERS
**********************************************************************/

void TransportLine::setMaxSpeed(SpeedUnit max_speed){
    this->max_speed = max_speed;
}

void TransportLine::setCrossingPrice(CurrencyUnit crossing_price){
    this->crossing_price = crossing_price;
}

void TransportLine::setTemporarilyClosed(bool closed){
    this->temporarily_closed = closed;
    this->getStyle()->color = this->getStyle()->border_color =
            ( this->temporarily_closed || this->permanently_closed ) ? QColor("Red") : QColor("Green");
}

void TransportLine::setPermanentlyClosed(bool closed){
    this->permanently_closed = closed;
    this->getStyle()->color = this->getStyle()->border_color =
            ( this->temporarily_closed || this->permanently_closed ) ? QColor("Red") : QColor("Green");
}

void TransportLine::setMaxCapacity(unsigned int capacity){
    foreach(Skill* s , this->getSkills( AgentStoreSkill::staticMetaObject.className() )){
        dynamic_cast<AgentStoreSkill*>(s)->setMaxCapacity(capacity);
    }
}

/**********************************************************************
 METHODS
**********************************************************************/

bool TransportLine::enter( Agent* entering_agent ){

    entering_agent->setGeometry( this->getGeometry()->createCentroid() );

    // Bikes, Motorbikes and people dont occupy road
    if( dynamic_cast<Bicycle*>( entering_agent) || dynamic_cast<Motorcycle*>( entering_agent ) || dynamic_cast<Person*>( entering_agent ) ){
        return true;
    }

    AgentStoreSkill* c = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className()) );
    if( c->enter( entering_agent ) ){
        return true;
    }

    Debugger::warning( this , Q_FUNC_INFO , QString("Entered a full road. Capacity %1 , inside %2").arg( c->getMaxCapacity() ).arg( c->countAgentsInside() ) );
    return false;
}

bool TransportLine::exit( Agent* exiting_agent ){
    AgentStoreSkill* c = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className()) );
    return c->exit( exiting_agent );
}
