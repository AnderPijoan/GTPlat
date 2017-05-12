#include "GTPlatScenarioCreator.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QFile>
#include <QThread>

#include "environment/Environment.h"

#include "agents/places/administrative_areas/AdministrativeArea.h"
#include "agents/places/constructions/accommodations/Accommodation.h"

#include "agents/transport_networks/walk_paths/WalkPath.h"
#include "agents/transport_networks/roads/Road.h"
#include "agents/transport_networks/public_transport/PublicTransportStretch.h"
#include "agents/transport_networks/elements/TrafficLight.h"

#include "agents/people/vehicle_drivers/PublicTransportVehicleDriver.h"
#include "agents/people/Person.h"
#include "agents/vehicles/Car.h"
#include "agents/vehicles/ECar.h"
#include "agents/vehicles/Bicycle.h"
#include "agents/vehicles/Motorcycle.h"

#include "skills/produce/pollution/vehicle/linear/VehicleLinearPollutionProduceSkill.h"
#include "skills/produce/pollution/vehicle/sumo/VehicleSumoPollutionProduceSkill.h"
#include "skills/produce/pollution/vehicle/exp/VehicleExpPollutionProduceSkill.h"
#include "skills/produce/pollution/vehicle/svm/VehicleSVMPollutionProduceSkill.h"

#include "usecases/gtplat/GTPlatAgent.h"

#include "utils/exporters/logger/Logger.h"
#include "utils/paralelism_controller/ParallelismController.h"
#include "utils/storage/SharedObjectStorage.h"
#include "utils/routing/GSSDijkstraRoutingGraph.h"


GTPlatScenarioCreator::GTPlatScenarioCreator(Environment* environment , WebSocketServer* ws_server) : QObject(){
    this->environment = environment;
    this->ws_server = ws_server;
    for(int i = 0 ; i < ParallelismController::getInstance()->getThreadsCount() ; i++){
        ASyncAPIDriver* api = new ASyncAPIDriver();
        api->moveToThread( ParallelismController::getInstance()->getThread(i) );
        this->api_drivers.append( api );
    }

    QObject::connect( environment , &Environment::agentStartedSignal , this , &GTPlatScenarioCreator::runScenario );
}

QJsonObject GTPlatScenarioCreator::toJSON(){
    QJsonObject json;

    // Profiles
    QJsonArray profiles;
    foreach(GTPlatProfile* p , this->profiles.values()){
        QJsonObject profile;
        profile.insert( "name" , p->objectName() );
        int amount = 0;
        foreach(GTPlatJourney* j , this->journeys){
            if( j->profile == p ){
                amount += j->amount;
            }
        }
        profile.insert( "amount" , amount );
        profiles.append( profile );
    }
    json.insert( "profiles" ,  profiles);

    // Incentives
    QJsonArray incentives;
    foreach(GTPlatIncentive* i , this->incentives.values()){
        incentives.append( i->toJSON() );
    }
    json.insert( "incentives" ,  incentives);

    return json;
}

void GTPlatScenarioCreator::loadScenario(){

    // LOAD PROFILES FROM SCENARO FOLDER
    Debugger::info( this , Q_FUNC_INFO , "Reading profiles");
    {
        QDir folder("./../usecases/gtplat/scenario/profiles");
        foreach( QString file_name , folder.entryList() ){

            QFile file( folder.absoluteFilePath( file_name ) );

            if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {

                QJsonParseError jerror;
                QJsonDocument json_document = QJsonDocument::fromJson( file.readAll() , &jerror);

                if(jerror.error != QJsonParseError::NoError){
                    Debugger::warning( this , Q_FUNC_INFO , QString("Error when parsing profiles JSON: %1").arg( jerror.errorString() ) );
                    continue;
                }

                QJsonArray pps = json_document.array();

                for(int i = 0; i < pps.size(); i++){
                    QJsonObject p = pps.at(i).toObject();
                    GTPlatProfile* profile = new GTPlatProfile( p );
                    this->profiles.insert( profile->objectName() , profile );
                    Debugger::info( this , Q_FUNC_INFO , QString("Created profile %1").arg( profile->objectName() ) );
                }

                file.close();
            }
        }
    }

    // CREATE ROUTING GRAPHS
    Debugger::info( this , Q_FUNC_INFO , "Creating routing graphs");
    this->createRoutingGraph( "WALK" , QStringList("WALK") );
    this->createRoutingGraph( "CAR" , QStringList("CAR") );
    this->createRoutingGraph( "ELECTRIC" , QStringList("ELECTRIC") );
    this->createRoutingGraph( "BICYCLE" , QStringList("BICYCLE") );
    this->createRoutingGraph( "MOTORCYCLE" , QStringList("MOTORCYCLE") );
    this->createRoutingGraph( "CAR" , QStringList("CAR") );

    // LOAD JOURNEYS FROM SCENARO FOLDER
    Debugger::info( this , Q_FUNC_INFO , "Reading journeys");
    {
        QDir folder("./../usecases/gtplat/scenario/journeys");
        foreach( QString file_name , folder.entryList() ){

            QFile file( folder.absoluteFilePath( file_name ) );

            if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {

                QJsonParseError jerror;
                QJsonDocument json_document = QJsonDocument::fromJson( file.readAll() , &jerror);

                if(jerror.error != QJsonParseError::NoError){
                    Debugger::warning( this , Q_FUNC_INFO , QString("Error when parsing profiles JSON: %1").arg( jerror.errorString() ) );
                    continue;
                }

                QJsonArray jjs = json_document.array();

                for(int i = 0; i < jjs.size(); i++){

                    QJsonObject j = jjs.at(i).toObject();
                    GTPlatJourney* journey = new GTPlatJourney();
                    journey->fromJSON( j );
                    journey->profile = this->profiles.value( j.value( "profile" ).toString() );
                    if( !journey->profile ){
                        Debugger::critical( this , Q_FUNC_INFO , QString("Error trying to create journey for profile %1").arg( j.value("profile").toString() ) );
                    }
                    journey->from = dynamic_cast<AdministrativeArea*>( environment->getByClassAndName( AdministrativeArea::staticMetaObject.className() , j.value("from").toString() ) );
                    if( !journey->from ){
                        Debugger::critical( this , Q_FUNC_INFO , QString("Error journey boundary %1 not found").arg( j.value("from").toString() ) );
                    }
                    journey->to = dynamic_cast<AdministrativeArea*>( environment->getByClassAndName( AdministrativeArea::staticMetaObject.className() , j.value("to").toString() ) );
                    if( !journey->from ){
                        Debugger::critical( this , Q_FUNC_INFO , QString("Error journey boundary %1 not found").arg( j.value("to").toString() ) );
                    }
                    this->journeys.append( journey );
                }

                file.close();
            }
        }
    }


    // LOAD INCENTIVES FROM SCENARO FOLDER
    Debugger::info( this , Q_FUNC_INFO , "Reading incentives" );
    {
        QDir folder("./../usecases/gtplat/scenario/incentives");
        foreach( QString file_name , folder.entryList() ){

            QFile file( folder.absoluteFilePath( file_name ) );

            if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {

                QJsonParseError jerror;
                QJsonDocument json_document = QJsonDocument::fromJson( file.readAll() , &jerror);

                if(jerror.error != QJsonParseError::NoError){
                    Debugger::warning( this , Q_FUNC_INFO , QString("Error when parsing incentives JSON: %1").arg( jerror.errorString() ) );
                    continue;
                }

                QJsonArray iis = json_document.array();

                for(int i = 0; i < iis.size(); i++){

                    QJsonObject j = iis.at(i).toObject();
                    GTPlatIncentive* incentive = new GTPlatIncentive();
                    incentive->fromJSON( j );
                    this->incentives.insert( incentive->name , incentive );
                }

                file.close();
            }
        }
    }
}

void GTPlatScenarioCreator::runScenario( QString class_name , int id ){
    Q_UNUSED( class_name );
    Q_UNUSED( id );

    Debugger::info( this , Q_FUNC_INFO , "Starting scenario");

    // DELETE OLD AGENTS
    foreach(Agent* a , environment->getByClass( GTPlatAgent::staticMetaObject.className() )){
        environment->removeAgent( a );
        a->deleteLater();
    }

    // CREATE ROULETTES
    QMap<QString , RouletteWheel*> roulettes;
    Debugger::info( this , Q_FUNC_INFO , "Creating random start-end locations generator");
    foreach(Agent* a , environment->getByClass( AdministrativeArea::staticMetaObject.className() )){
        roulettes.insert( a->objectName() , new RouletteWheel() );

        foreach(Agent* b , environment->getPhysicalEnvironment()->getAgentsIntersecting( a->getGeometry() , Accommodation::staticMetaObject.className() ) ) {

            Accommodation* building = dynamic_cast<Accommodation*>( b );
            roulettes[ a->objectName() ]->addElement( building , building->getLevels() * building->getArea().number() );
        }
    }

    // APPLY INCENTIVES
    //TODO

    // CREATE AGENTS
    foreach(GTPlatJourney* journey, this->journeys){

        Debugger::info( this , Q_FUNC_INFO , QString( "Creating %1 profiles %2 with options : %3").arg( journey->amount ).arg( journey->profile->objectName() ).arg( journey->profile->options.join(',') ) );

        for(int i = 0; i < journey->amount * journey->multiply_by ; i++){

            srand( i );
            QDateTime departure;
            departure.setTime( journey->time );
            departure.setDate( journey->date );

            GTPlatAgent* agent = new GTPlatAgent( journey->profile , departure, PersonConstants::MALE );
            departure = departure.addMSecs( agent->getId() * qrand() % qMax( 1 , journey->time_deviation * 1000) );
            agent->setNextTick( departure );
            agent->start_time = departure;
            agent->scenario_creator = this;
            {
                if( !roulettes.contains( journey->from->objectName() ) ){
                    Debugger::warning( this , Q_FUNC_INFO , QString("No boundary found named %1").arg( journey->from->objectName() ) );
                }
                Accommodation* building = dynamic_cast<Accommodation*>( roulettes[ journey->from->objectName() ]->getElement() );
                if( building ){
                    agent->departure_place = building->getRepresentativeCoordinate();
                } else {
                    agent->departure_place = journey->from->getRepresentativeCoordinate();
                }
                agent->setGeometry( environment->getPhysicalEnvironment()->getGeometryController()->createPoint( agent->departure_place ) );
            }

            {
                if( !roulettes.contains( journey->to->objectName() ) ){
                    Debugger::warning( this , Q_FUNC_INFO , QString("No boundary found named %1").arg( journey->from->objectName() ) );
                }
                Accommodation* building = dynamic_cast<Accommodation*>( roulettes[ journey->to->objectName() ]->getElement() );
                if( building ){
                    agent->destination_place = building->getRepresentativeCoordinate();
                } else {
                    agent->destination_place = journey->to->getRepresentativeCoordinate();
                }
            }

            environment->addAgent( agent );
            this->to_be_run.insert( agent , departure );
        }
    }

    // RUN TRAFFIC LIGHTS
    Debugger::info( this , Q_FUNC_INFO , "Running Traffic Lights");
    {
        QList<Agent*> as = environment->getByClass( TrafficLight::staticMetaObject.className() );
        foreach(Agent* a , as ){
            environment->runAgent( a );
        }
    }

    // RUN PUBLIC TRANSPORT
    Debugger::info( this , Q_FUNC_INFO , "Running Public Transport");
    {
        QList<Agent*> as = environment->getByClass( PublicTransportVehicleDriver::staticMetaObject.className() );
        foreach(Agent* a , as ){
            environment->runAgent( a );
        }
    }

    Debugger::info( this , Q_FUNC_INFO , "Starting execution (This may take a while...)" );

    QTimer::singleShot( 0 , this , &GTPlatScenarioCreator::cycle );
}

void GTPlatScenarioCreator::cycle(){

    // RUN TO_BE_RUN AGENTS
    {
        QDateTime now = this->environment->getTimeEnvironment()->getCurrentDateTime();
        QList<GTPlatAgent*> as = this->to_be_run.keys();
        foreach(GTPlatAgent* a , as ){
            if( this->to_be_run.value( a ) < now ){
                to_be_run.remove( a );
                environment->runAgent( a );
            }
        }
    }

    try {

        if( !this->ws_server ){ return; }

        // PREPARE RESULTS DATA

        QJsonObject results;
        results.insert( "current_modes" , CustomJsonExporter::format( this->current_using_leg_mode , "Current moving agents" ) );
        results.insert( "chosen_modes" , CustomJsonExporter::format( this->global_chosen_mode , "Chosen citizen modes" ) );
        results.insert( "impact_cc" , CustomJsonExporter::format( this->global_impact_cc , "Global Climate Change Impact" ) );

        this->ws_server->pushJson( results , "RESULTS" );

    } catch(...){}

    QTimer::singleShot( 5000 , this , &GTPlatScenarioCreator::cycle );
}


void GTPlatScenarioCreator::createRoutingGraph(QString graph_name , QStringList modes ){

    if( this->routing_graphs.keys().contains( graph_name ) ){ return; }

    const GSSGraph* walk_original = this->environment->getNetworkEnvironment()->getGraph( WalkPath::staticMetaObject.className() );
    const GSSGraph* road_original = this->environment->getNetworkEnvironment()->getGraph( Road::staticMetaObject.className() );
    const GSSGraph* transit_original = this->environment->getNetworkEnvironment()->getGraph( PublicTransportStretch::staticMetaObject.className() );

    QMap<GSSGraphEdge*,double> cost_map;

    if( modes.contains( "WALK" ) && walk_original ){
        foreach(GSSGraphEdge* e , walk_original->getEdges()){
            cost_map.insert( e , e->getCost() );
        }
    }

    if( modes.contains( "BICYCLE" ) && road_original){
        foreach(GSSGraphEdge* e , road_original->getEdges()){
            if( dynamic_cast<TransportLine*>( e )->isAllowed( Bicycle::staticMetaObject.className() ) ){
                cost_map.insert( e , e->getCost() );
            }
        }
    }

    if( modes.contains( "ELECTRIC" ) && road_original){
        foreach(GSSGraphEdge* e , road_original->getEdges()){
            if( dynamic_cast<TransportLine*>( e )->isAllowed( Motorcycle::staticMetaObject.className() ) ){
                cost_map.insert( e , e->getCost() );
            }
        }
    }

    if( modes.contains( "MOTORCYCLE" ) && road_original){
        foreach(GSSGraphEdge* e , road_original->getEdges()){
            if( dynamic_cast<TransportLine*>( e )->isAllowed( ECar::staticMetaObject.className() ) ){
                cost_map.insert( e , e->getCost() );
            }
        }
    }

    if( modes.contains( "CAR" ) && road_original){
        foreach(GSSGraphEdge* e , road_original->getEdges()){
            if( dynamic_cast<TransportLine*>( e )->isAllowed( Car::staticMetaObject.className() ) ){
                cost_map.insert( e , e->getCost() );
            }
        }
    }

    if( modes.contains( "TRANSIT" ) && transit_original ){
        foreach(GSSGraphEdge* e , transit_original->getEdges()){
            cost_map.insert( e , e->getCost() );
        }
    }

    GSSDijkstraRoutingGraph* drg = new GSSDijkstraRoutingGraph( cost_map );
    drg->setObjectName( graph_name );
    this->routing_graphs.insert( graph_name , drg );
}
