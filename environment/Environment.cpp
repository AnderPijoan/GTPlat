#include "Environment.h"

#include <QPainter>
#include <QTimer>
#include "utils/paralelism_controller/ParallelismController.h"

/**
 * @brief Environment::Environment
 */
Environment::Environment(QCoreApplication* parent) : Agent( 0 , parent )
{

    // Init sub environments
    this->execution_environment = new ExecutionEnvironment( this );
    this->physical_environmant = new PhysicalEnvironment( this );
    this->social_environment = new SocialEnvironment( this );
    this->time_environment = new TimeEnvironment( this );
    this->climate_environment = new ClimateEnvironment( this );
    this->network_environment = new NetworkEnvironment( this );

    // IMPORTANT, SET SELF TO 0
    this->environment = 0;

    // Storage
    this->environment_agents = new AgentStorage( this );

    // Random generators
    qsrand( QDateTime::currentDateTime().time().second() );

    // Style
    this->getStyle()->color = QColor(255,255,255,0);

    Debugger::info( this , Q_FUNC_INFO , "Environment created" );
}

Environment::~Environment(){
    this->execution_environment->deleteLater();
    this->physical_environmant->deleteLater();
    this->social_environment->deleteLater();
    this->time_environment->deleteLater();
    this->climate_environment->deleteLater();
    this->network_environment->deleteLater();

    this->environment_agents->deleteLater();

    Debugger::info( this , Q_FUNC_INFO , "Environment deleted" );
}


/**********************************************************************
 EXPORTERS
**********************************************************************/

QJsonObject Environment::toJSON(){
    QJsonObject json = Agent::toJSON();
    QJsonArray agent_classes;
    foreach ( QString class_name, this->getClasses() ) {
        agent_classes.append( class_name );
    }
    json.insert("agent_types", agent_classes );
    json.insert( "time" , this->time_environment->toJSON() );
    return json;
}

/**
 * @brief Agent::toImage
 * @return
 */
QImage Environment::toImage( const GSSEnvelope image_bounds , unsigned int image_width, unsigned int image_height, QStringList class_names){

    QImage env_image = Agent::toImage( image_bounds , image_width , image_height );

    QPainter painter( &env_image );

    foreach( QString class_name , class_names) {
        foreach(QObject* o , this->environment_agents->getByClass( class_name )){
            Agent* a = dynamic_cast<Agent*>( o );
            if( a ){
                painter.drawImage( 0 , 0 , a->toImage( image_bounds , image_width , image_height ) );
            }
        }
    }
    return env_image;
}

/**********************************************************************
 IMPORTERS
**********************************************************************/

void Environment::fromJSON(QJsonObject json){
    Agent::fromJSON(json);
    QJsonObject properties = json["properties"].toObject();
    if( properties.contains( "time" ) ){
        this->time_environment->fromJSON( properties["time"].toObject() );
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

GSSCoordinate Environment::getRandomPoint(GSSGeometry* bounds, unsigned int seed) const{
    GSSCoordinate p = this->physical_environmant->getGeometryController()->getRandomPoint(bounds ? bounds : this->getGeometry() , seed);
    return p;
}

const ExecutionEnvironment* Environment::getExecutionEnvironment() const{
    return this->execution_environment;
}

const PhysicalEnvironment* Environment::getPhysicalEnvironment() const{
    return this->physical_environmant;
}

const SocialEnvironment* Environment::getSocialEnvironment() const{
    return this->social_environment;
}

const TimeEnvironment* Environment::getTimeEnvironment() const{
    return this->time_environment;
}

const ClimateEnvironment* Environment::getClimateEnvironment() const{
    return this->climate_environment;
}

const NetworkEnvironment* Environment::getNetworkEnvironment() const{
    return this->network_environment;
}


/**********************************************************************
 AGENTS METHODS
**********************************************************************/

bool Environment::addAgents(QList<Agent*> agents){
    QListIterator<Agent*> i(agents);
    bool inserted = true;
    while(i.hasNext()){
        inserted &= this->addAgent( i.next() );
    }
    return inserted;
}

bool Environment::addAgent(Agent *agent){

    if( !agent || agent->environment != 0 ){ return false; }

    // Memorize
    this->environment_agents->add( agent );

    // Add to environments
    this->physical_environmant->addAgent( agent );
    this->social_environment->addAgent( agent );
    this->network_environment->addAgent( agent );

    // Set the agents environment
    agent->environment = this;

    return true;
}

bool Environment::removeAgent(Agent* agent){

    if( !agent ){ return false; }

    // Forget
    this->environment_agents->remove( agent );

    // Delete from environments
    this->physical_environmant->removeAgent( agent );
    this->social_environment->removeAgent( agent );
    this->network_environment->removeAgent( agent );

    // Set the agents environment
    agent->environment = 0;

    return true;
}

bool Environment::runAgent( Agent *agent ){
    if( !agent || agent->environment != this ){
        Debugger::info( this , Q_FUNC_INFO , "Agent is not in this environment" );
        return false;
    }
    this->execution_environment->addAgent( agent );
    return true;
}

bool Environment::stopAgent(Agent *agent){
    if( !agent || agent->environment != this ){
        Debugger::info( this , Q_FUNC_INFO , "Agent is not in this environment" );
        return false;
    }
    this->execution_environment->removeAgent( agent );
    return true;
}

bool Environment::setAgentGeometry(Agent *agent, GSSGeometry *new_geom){

    // Send it to PhysicalEnvironment
    return this->physical_environmant->setAgentGeometry( agent , new_geom );
}


/* GETTERS
**********************************************************************/

const QStringList Environment::getClasses() const{
    return this->environment_agents->getClasses();
}

Agent* Environment::getByName(QString agent_name) const{
    return dynamic_cast<Agent*>( this->environment_agents->getByName(agent_name) );
}

Agent* Environment::getByClassAndId(QString class_name , unsigned int id) const{
    return this->environment_agents->getByClassAndId( class_name , id );
}

Agent* Environment::getByClassAndName(QString class_name , QString agent_name) const{
    return this->environment_agents->getByClassAndName( class_name , agent_name );
}

Agent* Environment::getById(unsigned int id) const{
    return this->environment_agents->getById( id );
}

QList<Agent*> Environment::getByClass(QString class_name) const{
    return this->environment_agents->getAgentsByClass( class_name );
}

/**********************************************************************
 SLOTS
**********************************************************************/

void Environment::runEnvironment(){
    this->run();
    this->timer->singleShot( 1000 , Qt::CoarseTimer , this , &Agent::behaveWrapper );
}

void Environment::stopEnvironment(){
    this->stop();
}

void Environment::behave(){

    if( !this->running ){ return; }

    // Keep start time
    QDateTime start = QDateTime::currentDateTime();

    // Execute agents
    this->execution_environment->executeAgents( this->time_environment );

    // Once the loop has been iterated, schedule next behave() call
    this->timer->deleteLater();
    this->timer = new QTimer( this );

    // Call slot in (cycleFrequency - spent time) time
    double msecs = qMax( 1.0 , this->execution_environment->getCycleFrequency() - start.msecsTo( QDateTime::currentDateTime() ) );
    this->timer->singleShot( msecs , Qt::CoarseTimer , this , &Environment::behaveWrapper );
}
