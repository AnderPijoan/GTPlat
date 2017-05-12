#include "Agent.h"

#include <QMetaProperty>
#include <QSqlField>
#include <QJsonValue>
#include <QJsonDocument>
#include <QPainter>
#include <QThread>

#include "environment/Environment.h"

#include "utils/importers/geojson_to_geometry_importer/GeoJsonToGeometry.h"
#include "utils/paralelism_controller/ParallelismController.h"
#include "utils/image_downloader/ImageDownloader.h"

#include "skills/Skill.h"

unsigned int Agent::counter = 0;

Agent::Agent(GSSGeometry* geom , QObject* parent) : QObject( parent ) , id(++Agent::counter) , geometry(geom) , running(false) , busy(false) {
    Debugger::debug( this , Q_FUNC_INFO , "Created");

    // Name
    this->setObjectName( QString("%1 %2").arg( this->metaObject()->className() ).arg( this->getId() ) );

    // Style
    this->style = new UiStyle(this);

    // Init skills container
    this->skills = new ObjectStorage( this );
}

Agent::Agent( QJsonObject json , QObject* parent ) : QObject( parent) , id(++Agent::counter) , geometry(0) , running(false) , busy(false) {
    Q_UNUSED(json);
}

Agent::~Agent() {
    Debugger::debug( this , Q_FUNC_INFO , "Deleted");
    if( this->running ){ this->stop(); }
    if( this->environment ){ this->environment->removeAgent( this ); }
    if( this->style ){ this->style->deleteLater(); }
    if( this->geometry ){ this->geometry->deleteLater(); }
    if( this->timer ){ this->timer->deleteLater(); }

    qDeleteAll( this->skills->getByClass( QObject::staticMetaObject.className() ) );
    this->skills->deleteLater();
}

/**********************************************************************
 EXPOERTERS
**********************************************************************/

/**
 * @brief Agent::toMiniJSON Called when asked for many agents.
 * So only contains basic attributes and style.
 * Can be overwritten if want to add extra variables to child classes
 * @return
 */
QJsonObject Agent::toMiniJSON(){

    QJsonObject agent_json;

    agent_json.insert( "id" , QString::number( this->getId() ) );
    agent_json.insert( "type" , this->metaObject()->className() );
    agent_json.insert( "name" , CustomJsonExporter::format( this->objectName() ) );

    QJsonObject json_style = this->style->toJSON();
    foreach(QString key , json_style.keys() ){
        agent_json.insert( key , json_style.value( key ).toObject() );
    }

    this->mutex.lock();
    if( this->geometry ){
        agent_json.insert( "geometry" , CustomJsonExporter::format( this->geometry ) );
    }
    this->mutex.unlock();

    return agent_json;
}

/**
 * @brief Agent::toJSON Called when asked for more info about this agent.
 * Contains all the additional information of the agent.
 * Can be overwritten if want to add extra variables to child classes
 * @return
 */
QJsonObject Agent::toJSON(){
    return this->toMiniJSON();
}

/**
 * @brief Agent::toImage
 * @return
 */
QImage Agent::toImage(const GSSEnvelope image_bounds, unsigned int image_width, unsigned int image_height ){

    /*if( this->getStyle()->icon.isNull() ){
        this->getStyle()->icon = ImageDownloader::downloadImage( this->getStyle()->icon_url );
    }*/

    return this->geometry->toImage( image_bounds , image_width , image_height , this->getStyle()->border_color , this->getStyle()->border_weight , this->getStyle()->color , this->getStyle()->icon );
}


/**********************************************************************
 IMPORTERS
**********************************************************************/

/**
 * @brief Agent::fromJSON Update agent from JsonObject sent from frontend or importer
 * @param json {geometry : {} , properties : {}}
 */
void Agent::fromJSON(QJsonObject json){

    if( json.value("id").toInt() != (int)this->getId() || json.value("type").toString() != this->metaObject()->className() ){
        return;
    }

    // TODO
    if( json.contains("name") ){ this->setObjectName( json.value("name").toString() ); }
}

void Agent::fromSQLRecord(QSqlRecord record){
    QJsonObject json;
    for( int i = 0; i < record.count() ; i++){
        QJsonValue val = record.field(i).value().toJsonValue();
        json.insert( record.field(i).name() , val );
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

unsigned int Agent::getId() const{
    return this->id;
}

bool Agent::isRunning() const{
    return this->running;
}

const GSSGeometry* Agent::getGeometry() const{
    return this->geometry;
}

/**
 * @brief Agent::getCoordinate
 * Representative Coordinate of this agents location, USED FOR GRAPHS AND ROUTING
 * @return
 */
GSSCoordinate Agent::getRepresentativeCoordinate() const{
    if( this->geometry ){
        return this->geometry->getRepresentativeCoordinate();
    }
    return GSSCoordinate(0,0,0);
}

QDateTime Agent::getBornDatetime() const{
    return this->born_datetime;
}

UiStyle* Agent::getStyle() const{
    return this->style;
}

Environment* Agent::getEnvironment() const{
    return this->environment;
}

bool Agent::hasSkill(QString class_name) const{
    return this->skills->contains( class_name );
}

Skill* Agent::getSkill(QString class_name) const{
    const QList<QObject*> objs = this->skills->getByClass( class_name );
    if( objs.isEmpty() ){
        Debugger::debug( this , Q_FUNC_INFO , QString("Has no skill %1").arg( class_name ) );
        return 0;
    }
    return dynamic_cast<Skill*>( objs.at(0) );
}

QList<Skill*> Agent::getSkills(QString class_name) const{
    QList<Skill*> s;
    foreach(QObject* obj , this->skills->getByClass( class_name )){
        s.append( dynamic_cast<Skill*>( obj ) );
    }
    if( s.isEmpty() ){
        Debugger::debug( this , Q_FUNC_INFO , QString("Has no skills %1").arg( class_name ) );
    }
    return s;
}


/**********************************************************************
 SETTERS
**********************************************************************/

/**
 * @brief GraphNode::setGeometry
 * WARNING, agent will take ownership of the geometry, Do not use
 * shared geometries, make a clone before passing it to agent!!
 * WARNING, Too much concurrency, always ensure the agent has a geometry
 * @param geom
 */
void Agent::setGeometry(GSSGeometry* new_geometry){

    if( this->geometry == new_geometry ){ return; }

    if( this->environment ){

        // Environment ensures geometry correction and index, etc.
        this->environment->setAgentGeometry( this , new_geometry );

    } else {

        // Can have whatever geometry
        this->mutex.lock();
        if( this->geometry ){
            this->geometry->deleteLater();
            this->geometry = 0;
        }
        if( new_geometry ){
            this->geometry = new_geometry->createCopy();
            new_geometry->deleteLater();
        }
        this->mutex.unlock();
    }
}

void Agent::setNextTick(QDateTime datetime){
    this->next_tick_datetime = datetime;
}

void Agent::setNextTick( TimeUnit seconds ){
    // Min 0.0001 seconds
    TimeUnit checked_seconds = TimeUnit( qMax( 0.0001 , seconds.number() ) );
    this->next_tick_datetime = this->environment ?
                this->environment->getTimeEnvironment()->getCurrentDateTime().addSecs( checked_seconds.number() ) :
                QDateTime::fromString("INVALID");
}

void Agent::addSkill(Skill *skill){
    this->skills->add( skill );
}

void Agent::removeSkill(Skill *skill){
    this->skills->remove( skill );
}

/**********************************************************************
 SLOTS
**********************************************************************/

void Agent::run(){
    Debugger::debug( this , Q_FUNC_INFO , "Preparing to be run" );

    if( this->running ){
        Debugger::info( this , Q_FUNC_INFO , "Is already running" );
        return;
    }

    this->busy = true;

    // Create agent's own timer to schedule its slots
    if( !this->timer ){
        this->timer = new QTimer( this ); // Set its parent to this agent, Really improves speed
        this->timer->setSingleShot( true ); // Set single shot, really improves speed
    }

    double msecs = 1000;
    if( this->environment && this->next_tick_datetime.isValid() ){
        msecs = qMax( (qint64)0 , this->environment->getTimeEnvironment()->getCurrentDateTime().msecsTo( this->next_tick_datetime ) );
        msecs = msecs / this->environment->getTimeEnvironment()->getTimeSpeed();
    }

    // Non blocking method (if next_tick_datetime is invalid, msecs will be 1000 and it will be run instantly)
    this->timer->singleShot( msecs , Qt::CoarseTimer , [this](){

        Debugger::debug( this , Q_FUNC_INFO , "Running" );

        // Balance between threads. Only QObjects without parent can be moved. Children must stay in parents thread
        // Very slow operation
        if( !this->parent() ){
            Debugger::debug( this , Q_FUNC_INFO , "Moving to a parallel thread" );
            this->moveToThread( ParallelismController::getInstance()->getThread( this->getId() ) );
            this->timer->moveToThread( ParallelismController::getInstance()->getThread( this->getId() ) );
        }

        this->running = true;
        emit agentStartedSignal( this->metaObject()->className() , this->getId() );
        this->busy = false;
    });
}

/**
 * This method is a wrapper slot to be invoked by the Environment for behave() to be executed in the agents thread.
 **/
void Agent::behaveWrapper(){
    Debugger::debug( this , Q_FUNC_INFO , "Behaving" );

    this->busy = true;
    emit this->agentBehavedSignal( this->metaObject()->className() , this->getId() );

    this->setNextTick(); // In case behave() forgets to set it. Nevertheless, behave() can set a later tick time
    this->behave();
    this->busy = false;
}

void Agent::stop(){
    Debugger::debug( this , Q_FUNC_INFO , "Stopped" );

    this->busy = true;
    this->running = false;
    emit agentEndedSignal( this->metaObject()->className() , this->getId() );

    if( this->timer ){
        this->timer->deleteLater();
        this->timer = 0;
    }

    this->busy = false;
}
