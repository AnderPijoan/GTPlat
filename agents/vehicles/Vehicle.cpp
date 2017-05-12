#include "Vehicle.h"

#include "utils/geometry/GSSPoint.h"
#include "agents/transport_networks/TransportLine.h"
#include "environment/Environment.h"

#include "skills/produce/pollution/vehicle/VehiclePollutionProduceSkill.h"

Vehicle::Vehicle(GSSGeometry* geom , QObject* parent) : Agent( geom , parent ) {
    this->addSkill( new MoveSkill(this) );
    this->addSkill( new AgentStoreSkill(this) );
}

Vehicle::~Vehicle(){
    // Liberate road, to let other vehicles go by
    if( this->current_edge ){
        this->current_edge->exit( this );
        this->current_edge = 0;
    }
}

QJsonObject Vehicle::toJSON(){
    QJsonObject json = Agent::toJSON();
    QJsonObject attributes = json["attributes"].toObject();
    json.insert("attributes" , attributes);
    return json;
}

void Vehicle::fromJSON(QJsonObject json){
    Agent::fromJSON(json);
    if( json.contains( "properties" ) ){
        QJsonObject properties = json["properties"].toObject();
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

SpeedUnit Vehicle::getMaxSpeed() const{
    SpeedUnit avg = SpeedUnit( 0 , KilometerPerHour );
    foreach (Skill* s, this->getSkills( MoveSkill::staticMetaObject.className() ) ) {
        if( avg.number() == 0 ){
            avg = dynamic_cast<MoveSkill*>( s )->getMaxSpeed();
        } else {
            avg = ( avg + dynamic_cast<MoveSkill*>( s )->getMaxSpeed() ) / 2;
        }
    }
    return avg;
}

CurrencyUnit Vehicle::getUsageCost() const{
    return this->usage_cost;
}

const QList<Agent*> Vehicle::getAgentsInside() const{
    return dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className() ) )->getAgentsInside();
}

/**********************************************************************
 METHODS
**********************************************************************/

/**
 * @brief Vehicle::travelThrough Tell the car we are traveling through a road
 * @param road Road from where to copy its geometry
 * @param traffic Traffic situation to use in calculations
 */
void Vehicle::setTravellingThrough(TransportLine* edge , double traffic_level){
    this->current_edge = edge;
    this->traffic_level = traffic_level;

    // Set vehicle geometry
    this->setGeometry( edge->getGeometry()->createCentroid() );

    // Pollute
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    if( this->current_edge ){
        foreach(Skill* s , this->getSkills( VehiclePollutionProduceSkill::staticMetaObject.className() )){
            dynamic_cast<VehiclePollutionProduceSkill*>( s )->produce( RoadConstants::MOTORWAY , move_skill->getCurrentSpeed() , this->traffic_level , this->current_edge->getGradient() , 0.5 , this->current_edge->getLength() );
        }
    }

    // Set inner agents geometry
    AgentStoreSkill* skill = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className() ) );
    foreach(Agent* a , skill->getAgentsInside()){
        a->setGeometry( this->getGeometry()->createCopy() );
    }
}

bool Vehicle::enter(Agent *entering_agent){
    foreach(Skill* s , this->getSkills( AgentStoreSkill::staticMetaObject.className() )){
        if( dynamic_cast<AgentStoreSkill*>(s)->enter( entering_agent ) ){
            entering_agent->setGeometry( this->getGeometry()->createCopy() );
            return true;
        }
    }
    return false;
}

bool Vehicle::exit(Agent *exiting_agent){
    foreach(Skill* s , this->getSkills( AgentStoreSkill::staticMetaObject.className() )){
        dynamic_cast<AgentStoreSkill*>(s)->exit( exiting_agent );
    }
    return true;
}

void Vehicle::clear(){
    foreach(Skill* s , this->getSkills( AgentStoreSkill::staticMetaObject.className() )){
        dynamic_cast<AgentStoreSkill*>(s)->clear();
    }
}

/**********************************************************************
 SLOTS
**********************************************************************/

void Vehicle::behave(){

    // Vehicle can move?
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    if( !move_skill ){
        Debugger::info( this , Q_FUNC_INFO , "Vehicle cant move, stoping agent" );
        this->environment->stopAgent( this );
        return;
    }

    if( this->current_edge && this->current_edge->getGradient() > 0 ){ move_skill->brake( 0.1 ); }
    if( this->current_edge && this->current_edge->getGradient() < 0 ){ move_skill->accelerate( 0.1 ); }
}
