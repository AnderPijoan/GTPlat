#include "GTPlatAgent.h"

#include <QJsonDocument>
#include <QThread>

#include "environment/Environment.h"
#include "usecases/gtplat/GTPlatVehicle.h"
#include "usecases/gtplat/GTPlatItinerary.h"

#include "utils/exporters/logger/Logger.h"
#include "utils/routing/GSSDijkstraRoutingGraph.h"
#include "utils/exporters/custom_json_exporter/CustomJsonExporter.h"

#include "agents/transport_networks/walk_paths/WalkPath.h"
#include "agents/transport_networks/railways/Railway.h"
#include "agents/transport_networks/roads/Road.h"
#include "agents/transport_networks/TransportLine.h"
#include "agents/places/constructions/civil_structures/public_transport_stops/PublicTransportStop.h"
#include "agents/transport_networks/public_transport/PublicTransportStretch.h"
#include "agents/places/constructions/civil_structures/charging_stations/ChargingStation.h"

#include "agents/vehicles/Car.h"
#include "agents/vehicles/Bicycle.h"
#include "agents/vehicles/ECar.h"
#include "agents/vehicles/Motorcycle.h"
#include "agents/vehicles/public_transport/PublicTransportVehicle.h"

#include "agents/places/constructions/civil_structures/public_transport_stops/BusStop.h"
#include "agents/places/constructions/civil_structures/public_transport_stops/RailwayStop.h"

#include "skills/produce/pollution/vehicle/svm/VehicleSVMPollutionProduceSkill.h"
#include "utils/fuzzy_logic/FuzzyLogic.h"

#include "usecases/gtplat/GTPlatScenarioCreator.h"


GTPlatAgent::GTPlatAgent(GTPlatProfile* profile, QDateTime departure, PersonConstants::gender gender , GSSGeometry* geom , QObject *parent) : Person( gender , geom , parent ){
    this->next_tick_datetime = departure;
    this->profile = profile;

    this->private_vehicle = 0;
    this->chosen_itinerary = 0;

    this->addSkill( new ViewRouteAheadSkill(this) );
    this->addSkill( new DriveSkill(this) );
}

GTPlatAgent::~GTPlatAgent(){
}

QJsonObject GTPlatAgent::toJSON(){
    // Create geometry with itinerary
    GSSGeometry* line = this->environment->getPhysicalEnvironment()->getGeometryController()->createLineString( this->departure_place , this->destination_place );
    GSSGeometry* union_geom = this->getGeometry()->createUnion( line );
    delete line;


    QJsonObject agent_json = Person::toJSON();
    agent_json.insert( "geometry" , union_geom->toGeoJSON() );
    union_geom->deleteLater();

    return agent_json;
}


/**********************************************************************
 SLOTS
**********************************************************************/

void GTPlatAgent::behave(){

    switch (status) {
    case 0:
        // Download itineraries
        this->requestItineraries();
        break;

    case 1:
        // Wait while itineraries
        this->waitForItineraries();
        break;

    case 2:

        // Calculate route to vehicle
        this->choseBestItinerary();
        break;

    case 3:

        // Walk untill arriving at vehicle
        this->setLegRoute();
        break;

    case 4:
        this->walkRouteEnded();
        break;

    case 5:

        this->followWalkRoute();
        break;

    case 6:

        this->followDriveRoute();
        emit this->environment->pushToInterfaceSignal( this->toMiniJSON() , "UPDATE" );
        break;

    case 7:
        this->driveRouteEnded();
        break;

    case 8:
        this->waitForPublicTransport();
        break;

    case 9:
        this->routeFinished();
        break;
    }
}

void GTPlatAgent::requestItineraries(){

    // Get api driver
    ASyncAPIDriver* api = this->scenario_creator->api_drivers.at( this->getId() % this->scenario_creator->api_drivers.size() );

    // If api is busy, wait for 1 second
    if( api->getCurrentRequests() > 2 ){
        this->setNextTick( TimeUnit(1) );
        return;
    }

    // Calculate Itineraries
    GSSCoordinate from = this->getRepresentativeCoordinate();
    GSSCoordinate to = this->destination_place;

    foreach(QString option , this->profile->options){
        QNetworkReply* reply = this->requestGTALGItinerary( api , from , to , option );
        if( reply ){
            this->itinerary_mode.insert( reply , option );
        }
    }

    this->status++;
    this->setNextTick();
}

void GTPlatAgent::waitForItineraries(){


    ASyncAPIDriver* api = this->scenario_creator->api_drivers.at( this->getId() % this->scenario_creator->api_drivers.size() );

    bool finished = true;
    foreach( QNetworkReply* reply , this->itinerary_mode.keys() ){
        finished = finished && api->isFinished( reply );
    }

    // Not all finished, wait 2 seconds
    if( !finished ){
        this->setNextTick( TimeUnit(2) );
        return;
    }

    // Finished, take all replies and go to select best
    foreach( QNetworkReply* reply , this->itinerary_mode.keys() ){
        api->removeReply( reply );
    }

    this->status++;
    this->setNextTick();
}

void GTPlatAgent::choseBestItinerary(){

    QMap<QString , GTPlatItinerary*> option_itineraries;

    // FAKE ITINERARY
    //option_itineraries.insert( "CAR" , new GTPlatItinerary( this->departure_place , this->destination_place ) );

    foreach( QNetworkReply* reply , this->itinerary_mode.keys() ){

        if( reply->error() != QNetworkReply::NoError ){
            reply->deleteLater();
            continue;
        }
        QString option = this->itinerary_mode.value( reply );

        GTPlatItinerary* itinerary = new GTPlatItinerary( reply->readAll() , this->scenario_creator );
        reply->deleteLater();

        itinerary->setObjectName( option );
        option_itineraries.insert( option , itinerary );
    }

    // If no itineraries, finish
    if( option_itineraries.isEmpty() ){
        this->status = 9;
        this->setNextTick();
        return;
    }

    // If only one itinerary, select that
    if( option_itineraries.size() == 1 ){
        this->chosen_itinerary = option_itineraries.first();
        this->chosen_itinerary->loadRoutes( this->scenario_creator , this->environment );
        this->printLog( "CHOSEN" );
        this->scenario_creator->global_chosen_mode.insert( this->chosen_itinerary->objectName() , this->scenario_creator->global_chosen_mode.value( this->chosen_itinerary->objectName() , 0 ) +1 );
        this->remaining_legs = this->chosen_itinerary->legs;
        this->start_time = this->environment->getTimeEnvironment()->getCurrentDateTime();
        status++;
        this->setNextTick();
        return;
    }

    // PROCESS ITINERARIES

    FuzzyLogic* fuzzy_logic = new FuzzyLogic( this );

    FuzzyLogic::OutputMembershipFunction output;
    output.output_name = "TRANSPORT";
    QStringList fuzzy_results;
    fuzzy_results.append( "BICYCLE" );
    fuzzy_results.append( "WALK" );
    fuzzy_results.append( "BUS" );
    fuzzy_results.append( "SUBWAY" );
    fuzzy_results.append( "RAIL" );
    fuzzy_results.append( "TRANSIT" ); // BUS + SUBWAY + RAIL
    fuzzy_results.append( "CAR" );
    fuzzy_results.append( "MOTORCYCLE" );
    fuzzy_results.append( "ELECTRIC" );
    fuzzy_results.append( "BICYCLE_RENT" );
    for(int i = 0 ; i < fuzzy_results.size() ; i++){
        FuzzyLogic::MemberShipFunctionTerm m;
        m.name = fuzzy_results.at( i );
        m.shape_points.append( i );
        m.shape_points.append( i+1 );
        output.terms.append( m );
    }

    // GRZEGORZ RULES
    QList<FuzzyLogic::Rule> rules;

    // CAR
    { // if CAR_PRICE is VERY_LOW then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        rules.append( rule );
    }
    { // if CAR_TIME is VERY_LOW and TRANSIT_TIME is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if CAR_TIME is VERY_LOW and TRANSIT_TIME is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "HIGH" } );
        rules.append( rule );
    }
    { // if CAR_TIME is LOW and (TRANSIT_TIME is VERY_HIGH ) then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if CAR_TIME is MEDIUM and TRANSIT_TIME is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if CAR_TIME is HIGH and TRANSIT_TIME is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if CAR_TIME is LOW and MOTORCYCLE_TIME is MEDIUM then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_TIME" , "MEDIUM" } );
        rules.append( rule );
    }
    { // if CAR_TIME is LOW and MOTORCYCLE_TIME is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_TIME" , "HIGH" } );
        rules.append( rule );
    }
    { // if CAR_TIME is LOW and MOTORCYCLE_TIME is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_TIME" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is VERY_LOW and TRANSIT_PRICE is VERY_LOW then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is VERY_LOW and TRANSIT_PRICE is LOW then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is VERY_LOW and TRANSIT_PRICE is MEDIUM then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is VERY_LOW and TRANSIT_PRICE is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "HIGH" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is VERY_LOW and TRANSIT_PRICE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is LOW and TRANSIT_PRICE is LOW then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is LOW and TRANSIT_PRICE is MEDIUM then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is LOW and TRANSIT_PRICE is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "HIGH" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is LOW and TRANSIT_PRICE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is MEDIUM and TRANSIT_PRICE is MEDIUM then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is MEDIUM and TRANSIT_PRICE is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "HIGH" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is MEDIUM and TRANSIT_PRICE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is HIGH and TRANSIT_PRICE is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "HIGH" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is HIGH and TRANSIT_PRICE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if CAR_PRICE is VERY_HIGH and TRANSIT_PRICE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if CAR_DISTANCE is HIGH and BICYCLE_DISTANCE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_DISTANCE" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "BICYCLE_DISTANCE" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if CAR_DISTANCE is MEDIUM and BICYCLE_DISTANCE is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_DISTANCE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "BICYCLE_DISTANCE" , "HIGH" } );
        rules.append( rule );
    }
    { // if CAR_DISTANCE is MEDIUM and BICYCLE_DISTANCE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_DISTANCE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "BICYCLE_DISTANCE" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_DISTANCE is not VERY_LOW and TRANSIT_DISTANCE is not LOW and TRANSIT_DISTANCE is not MEDIUM and TRANSIT_DISTANCE is not HIGH and TRANSIT_DISTANCE is not VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_DISTANCE" , "not VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_DISTANCE" , "not LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_DISTANCE" , "not MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_DISTANCE" , "not HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_DISTANCE" , "not VERY_HIGH" } );
        rules.append( rule );
    }


    // TRANSIT
    { // if TRANSIT_PRICE is VERY_LOW and CAR_PRICE is LOW then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "LOW" } );
        rules.append( rule );
    }
    { // if TRANSIT_PRICE is VERY_LOW and CAR_PRICE is MEDIUM then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        rules.append( rule );
    }
    { // if TRANSIT_PRICE is VERY_LOW and CAR_PRICE is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_PRICE is VERY_LOW and CAR_PRICE is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_PRICE is LOW and CAR_PRICE is MEDIUM then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        rules.append( rule );
    }
    { // if TRANSIT_PRICE is LOW and CAR_PRICE is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_PRICE is LOW and CAR_PRICE is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_PRICE is MEDIUM and CAR_PRICE is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_PRICE is MEDIUM and CAR_PRICE is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_PRICE is HIGH and CAR_PRICE is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is VERY_LOW and CAR_TIME is VERY_LOW then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_LOW" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is VERY_LOW and CAR_TIME is LOW then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is VERY_LOW and CAR_TIME is MEDIUM then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is VERY_LOW and CAR_TIME is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is VERY_LOW and CAR_TIME is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is LOW and CAR_TIME is LOW then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is LOW and CAR_TIME is MEDIUM then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is LOW and CAR_TIME is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is LOW and CAR_TIME is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is MEDIUM and CAR_TIME is MEDIUM then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is MEDIUM and CAR_TIME is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is MEDIUM and CAR_TIME is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is HIGH and CAR_TIME is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "HIGH" } );
        rules.append( rule );
    }
    { // if TRANSIT_TIME is HIGH and CAR_TIME is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        rules.append( rule );
    }

    // MOTORCYCLE
    { // if MOTORCYCLE_DISTANCE is VERY_LOW and CAR_PRICE is HIGH then TRANSPORT is MOTORCYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "MOTORCYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_DISTANCE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        rules.append( rule );
    }
    { // if MOTORCYCLE_DISTANCE is VERY_LOW and CAR_PRICE is VERY_HIGH then TRANSPORT is MOTORCYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "MOTORCYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_DISTANCE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        rules.append( rule );
    }
    { // if MOTORCYCLE_PRICE is MEDIUM and CAR_PRICE is HIGH then TRANSPORT is MOTORCYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "MOTORCYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        rules.append( rule );
    }
    { // if MOTORCYCLE_PRICE is MEDIUM and CAR_PRICE is VERY_HIGH then TRANSPORT is MOTORCYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "MOTORCYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        rules.append( rule );
    }

    // WALK
    { // if WALK_DISTANCE is VERY_LOW then TRANSPORT is WALK
        FuzzyLogic::Rule rule;
        rule.output_value = "WALK";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "WALK_DISTANCE" , "VERY_LOW" } );
        rules.append( rule );
    }
    { // if WALK_DISTANCE is LOW then TRANSPORT is WALK
        FuzzyLogic::Rule rule;
        rule.output_value = "WALK";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "WALK_DISTANCE" , "LOW" } );
        rules.append( rule );
    }

    // BICYCLE
    { // if BICYCLE_DISTANCE is VERY_LOW then TRANSPORT is BICYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "BICYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "BICYCLE_DISTANCE" , "VERY_LOW" } );
        rules.append( rule );
    }
    { // if BICYCLE_TIME is VERY_LOW then TRANSPORT is BICYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "BICYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "BICYCLE_TIME" , "VERY_LOW" } );
        rules.append( rule );
    }

    QList<FuzzyLogic::InputMemberShipFunction> inputs;
    QStringList ins;
    ins.append("PRICE");
    ins.append("TIME");
    ins.append("DISTANCE");
    ins.append("EMISSIONS");
    foreach( QString t , fuzzy_results ){
        foreach (QString i , ins) {
            FuzzyLogic::InputMemberShipFunction input;
            input.input_name = QString("%1_%2").arg( t ).arg( i );

            // Terms
            {
                FuzzyLogic::MemberShipFunctionTerm term;
                term.name = "VERY_LOW";
                term.shape_points.append( -1 );
                term.shape_points.append( 0 );
                term.shape_points.append( 0.25 );
                input.terms.append( term );
            }
            {
                FuzzyLogic::MemberShipFunctionTerm term;
                term.name = "LOW";
                term.shape_points.append( 0 );
                term.shape_points.append( 0.25 );
                term.shape_points.append( 0.5 );
                input.terms.append( term );
            }
            {
                FuzzyLogic::MemberShipFunctionTerm term;
                term.name = "MEDIUM";
                term.shape_points.append( 0.25 );
                term.shape_points.append( 0.5 );
                term.shape_points.append( 0.75 );
                input.terms.append( term );
            }
            {
                FuzzyLogic::MemberShipFunctionTerm term;
                term.name = "HIGH";
                term.shape_points.append( 0.5 );
                term.shape_points.append( 0.75 );
                term.shape_points.append( 1 );
                input.terms.append( term );
            }
            {
                FuzzyLogic::MemberShipFunctionTerm term;
                term.name = "VERY_HIGH";
                term.shape_points.append( 0.75 );
                term.shape_points.append( 1 );
                term.shape_points.append( 1.25 );
                input.terms.append( term );
            }
            inputs.append( input );
        }
    }

    fuzzy_logic->setParameters( inputs , rules , output );
    fuzzy_logic->configure("Minimum", "Maximum", "AlgebraicProduct", "Maximum", "LargestOfMaximum");

    double max_duration = 0;
    double max_price = 0;
    double max_pollution = 0;
    double max_length = 0;
    foreach( GTPlatItinerary* it , option_itineraries.values() ){
        if( !it->route_found ){ continue; }
        max_duration = qMax( max_duration , it->getExpectedDuration().number() );
        max_price = qMax( max_price , it->expected_price.number() );
        max_pollution = qMax( max_pollution , it->getExpectedPollutionCC() );
        max_length = qMax( max_length , it->getExpectedLength().number() );
    }

    foreach( QString option , option_itineraries.keys() ){
        GTPlatItinerary* it = option_itineraries.value( option );
        if( !it->route_found ){ continue; }

        QString price_name = option + "_PRICE";
        fuzzy_logic->setInputValue( price_name , max_price == 0 ? 0 : it->getExpectedPrice().number() / max_price );

        QString time_name = option + "_TIME";
        fuzzy_logic->setInputValue( time_name , max_duration == 0 ? 0 : it->getExpectedDuration().number() / max_duration );

        QString length_name = option + "_DISTANCE";
        fuzzy_logic->setInputValue( length_name , max_length == 0 ? 0 : it->getExpectedLength().number() / max_length );

        QString green_name = option + "_EMISSIONS";
        fuzzy_logic->setInputValue( green_name , max_pollution == 0 ? 0 : it->getExpectedPollutionCC() / max_pollution );
    }

    double r = fuzzy_logic->getOutputValue( "TRANSPORT" );

    fuzzy_logic->deleteLater();

    // If no itineraries, finish
    if( option_itineraries.isEmpty() ){
        this->status = 9;
        this->setNextTick();
        return;
    }
    if( r == r && r > -1 ){ // Avoid NaN
        Debugger::warning( this , Q_FUNC_INFO , QString::number( r ) );
        QString vs = fuzzy_results.at( qFloor(r) );
        this->chosen_itinerary = option_itineraries.value( vs );
    }
    if( r != r ){ // Nan case
        //Debugger::warning( this , Q_FUNC_INFO , "Nan? " + QString::number( r ) );
        this->chosen_itinerary = option_itineraries.values().at( 0 );
    }
    if( this->chosen_itinerary ){
        Debugger::debug( this , Q_FUNC_INFO , "Chosen " + QString::number( r ) + " which means " + this->chosen_itinerary->objectName() );
        this->chosen_itinerary->loadRoutes( this->scenario_creator , this->environment );
        this->printLog( "CHOSEN" );
        this->scenario_creator->global_chosen_mode.insert( this->chosen_itinerary->objectName() , this->scenario_creator->global_chosen_mode.value( this->chosen_itinerary->objectName() , 0 ) +1 );
        this->remaining_legs = this->chosen_itinerary->legs;
        this->start_time = this->environment->getTimeEnvironment()->getCurrentDateTime();
    }

    status++;
    this->setNextTick();
}

void GTPlatAgent::setLegRoute(){

    if( this->remaining_legs.isEmpty() ){
        this->status = 9;
        this->setNextTick();
        return;
    }

    GTPlatItinerary::Leg leg = this->remaining_legs.at(0);
    if( leg.start == leg.end ){
        // Go to next leg
        this->remaining_legs.removeAt(0);
        status = 3;
        this->setNextTick();
        return;
    }

    this->scenario_creator->current_using_leg_mode.insert( leg.mode , this->scenario_creator->current_using_leg_mode.value( leg.mode , 0 ) +1 );

    // See leg mode
    if( leg.mode == "WALK" ){

        this->walk_route_skill = new WalkPathRoutingSkill( this );
        this->walk_route_skill->setRoute( leg.edges );
        this->real_length = this->real_length + this->walk_route_skill->getRemainingLength();
        status = 5;
        this->setNextTick();
        return;
    }

    if( leg.mode == "ELECTRIC" || leg.mode == "BICYCLE" || leg.mode == "MOTORCYCLE" || leg.mode == "CAR" ){

        if( leg.mode == "ELECTRIC" ){ this->private_vehicle = new ECar( this->getGeometry()->createCopy() , this ); }
        if( leg.mode == "BICYCLE" ){ this->private_vehicle = new Bicycle( this->getGeometry()->createCopy() , this ); }
        if( leg.mode == "MOTORCYCLE" ){ this->private_vehicle = new Motorcycle( this->getGeometry()->createCopy() , this ); }
        if( leg.mode == "CAR" ){ this->private_vehicle = new Car( this->getGeometry()->createCopy() , this ); }

        if( !this->private_vehicle ){
            Debugger::info( this , Q_FUNC_INFO , "Agent has selected an Itinerary using a private vehicle that doesnt have" );
            this->environment->stopAgent( this );
            return;
        }

        this->private_vehicle->addSkill( new VehicleSVMPollutionProduceSkill( PollutionConstants::CC , KUnitConversion::Gram , this->private_vehicle ) );
        this->private_vehicle->addSkill( new VehicleSVMPollutionProduceSkill( PollutionConstants::DALY , KUnitConversion::Gram , this->private_vehicle ) );

        this->environment->addAgent( this->private_vehicle );

        // Get inside vehicle
        this->private_vehicle->enter( this );
        this->getStyle()->icon_url = this->private_vehicle->getStyle()->icon_url;
        //emit this->environment->interfaceSignal( this , "UPDATE" );

        // Start vehicle
        this->environment->runAgent( this->private_vehicle );
        this->vehicle_route_skill = new TransportLineRoutingSkill( this->private_vehicle , this );
        this->vehicle_route_skill->setRoute( leg.edges );

        this->real_length = this->real_length + this->vehicle_route_skill->getRemainingLength();
        this->real_price = this->real_price + ( this->real_length.number() * this->private_vehicle->getUsageCost().number() );

        status = 6;
        this->setNextTick();
        return;
    }

    if( leg.mode == "BUS" || leg.mode == "TRAIN" || leg.mode == "TRAM" || leg.mode == "SUBWAY" ){

        // Connect and wait
        leg.from_stop->addWaitingPerson( this );
        this->setGeometry( leg.from_stop->getGeometry()->createCopy() );
        this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/61/61298.png";

        qRegisterMetaType< QList<PublicTransportStop*> >("QList<PublicTransportStop*>");
        this->connect( leg.from_stop , &PublicTransportStop::departingPublicTransportSignal , this , &GTPlatAgent::departingPublicTransport );
    }
}

// WALKING

void GTPlatAgent::followWalkRoute(){

    // GTPLAT STORE AVG SPEED
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );

    // Go to next edge
    WalkPath* following_walk_path = this->walk_route_skill->getFollowingEdge();
    if( !following_walk_path ){

        // Finished routing
        if( this->walk_route_skill->getCurrentEdge() ){ this->walk_route_skill->getCurrentEdge()->exit( this ); }
        move_skill->brakeToStop();
        status = 4;
        this->setNextTick();

    } else {

        // Walk to the max speed;
        move_skill->accelerate( 1 );

        if( following_walk_path->isTemporarilyClosed() ){
            // Could not enter, retry in 1 second
            move_skill->brakeToStop();
            this->setNextTick( TimeUnit(2) );
            return;
        }

        // Exit previous
        if( this->walk_route_skill->getCurrentEdge() ){ this->walk_route_skill->getCurrentEdge()->exit( this ); }

        // Enter edge and point to next
        following_walk_path->enter( this );
        this->walk_route_skill->pointToFollowingEdge();

        this->setGeometry( environment->getPhysicalEnvironment()->getGeometryController()->createPoint( this->walk_route_skill->getCurrentEdge()->getToNode()->getCoordinate() ) );

        // Calculate when we will arrive at edge end
        TimeUnit seconds = TimeUnit( this->walk_route_skill->getCurrentEdge()->getLength().number() / move_skill->getCurrentSpeed().number() );
        // Set next tick time after seconds
        this->setNextTick( seconds );

        //emit this->environment->pushToInterfaceSignal( this->toMiniJSON() , "UPDATE" );
    }
}

void GTPlatAgent::walkRouteEnded(){

    // Stop
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    move_skill->brakeToStop();

    this->scenario_creator->current_using_leg_mode.insert( this->remaining_legs.at(0).mode , this->scenario_creator->current_using_leg_mode.value( this->remaining_legs.at(0).mode , 0 ) -1 );

    this->walk_route_skill->deleteLater();
    this->walk_route_skill = 0;

    this->remaining_legs.removeAt( 0 );
    this->status = 3;
    this->setNextTick();
}

// PRIVATE VEHICLE

void GTPlatAgent::followDriveRoute(){

    MoveSkill* vehicle_move_skill = dynamic_cast<MoveSkill*>( this->private_vehicle->getSkill( MoveSkill::staticMetaObject.className() ) );

    // Go to next edge
    TransportLine* following_transport_line = this->vehicle_route_skill->getFollowingEdge();
    if( !following_transport_line ){

        // Finished routing
        if( this->vehicle_route_skill->getCurrentEdge() ){ this->vehicle_route_skill->getCurrentEdge()->exit( this->private_vehicle ); }
        vehicle_move_skill->brakeToStop();
        status = 7;
        this->setNextTick();

    } else {

        if( following_transport_line->isTemporarilyClosed() ){
            // Could not enter, retry in 1 second
            vehicle_move_skill->brakeToStop();
            this->setNextTick( TimeUnit(2) );
            return;
        }

        // See if we have free road
        ViewRouteAheadSkill* view_skill = dynamic_cast<ViewRouteAheadSkill*>( this->getSkill( ViewRouteAheadSkill::staticMetaObject.className() ) );
        view_skill->updateViewingRoads( this->vehicle_route_skill->getRemainingRoute() );

        // We have currently no free distance to go on
        if( following_transport_line->isFull() && !following_transport_line->isAgentInside( this->private_vehicle ) ){
            Debugger::debug( this , Q_FUNC_INFO , "Road is full, waiting for it to be free" );
            vehicle_move_skill->brakeToStop();
            this->private_vehicle->getStyle()->border_color = this->private_vehicle->getStyle()->color = QColor("Red");
            this->setNextTick( TimeUnit(2) );
            return;
        }

        // Exit previous
        if( this->vehicle_route_skill->getCurrentEdge() ){ this->vehicle_route_skill->getCurrentEdge()->exit( this->private_vehicle ); }

        // Point to next and Enter edge and
        this->vehicle_route_skill->pointToFollowingEdge();
        this->vehicle_route_skill->getCurrentEdge()->enter( this->private_vehicle );
        this->private_vehicle->setTravellingThrough( this->vehicle_route_skill->getCurrentEdge() , view_skill->getTrafficLevel() );
        this->private_vehicle->setGeometry( this->vehicle_route_skill->getCurrentEdge()->getGeometry()->createCopy() );
        this->setGeometry( this->private_vehicle->getGeometry()->createCentroid() );


        // Decide drive speed according to traffic and free distance
        DriveSkill* drive_skill = dynamic_cast<DriveSkill*>( this->getSkill( DriveSkill::staticMetaObject.className() ) );
        double accelerate_force = drive_skill->calculateAccelerateForce( vehicle_move_skill->getCurrentSpeed() , this->vehicle_route_skill->getCurrentEdge()->getMaxSpeed() , view_skill->getFreeDistance() );
        double brake_force =  drive_skill->calculateBrakeForce( vehicle_move_skill->getCurrentSpeed() , this->vehicle_route_skill->getCurrentEdge()->getMaxSpeed() , view_skill->getFreeDistance() );
        vehicle_move_skill->brake( brake_force );
        vehicle_move_skill->accelerate( accelerate_force );

        this->private_vehicle->getStyle()->border_color = this->private_vehicle->getStyle()->color = QColor( brake_force * 255 , accelerate_force * 255 , 128 );

        // Calculate when we will arrive at edge end
        TimeUnit seconds = TimeUnit( this->vehicle_route_skill->getCurrentEdge()->getLength().number() / vehicle_move_skill->getCurrentSpeed().number() );
        // Set next tick time after seconds
        this->setNextTick( seconds );

        //emit this->environment->pushToInterfaceSignal( this->toMiniJSON() , "UPDATE" );
    }
}

void GTPlatAgent::driveRouteEnded(){

    // Stop vehicle
    MoveSkill* vehicle_move_skill = dynamic_cast<MoveSkill*>( this->private_vehicle->getSkill( MoveSkill::staticMetaObject.className() ) );
    vehicle_move_skill->brakeToStop();

    this->scenario_creator->current_using_leg_mode.insert( this->remaining_legs.at(0).mode , this->scenario_creator->current_using_leg_mode.value( this->remaining_legs.at(0).mode , 0 ) -1 );

    this->vehicle_route_skill->deleteLater();
    this->vehicle_route_skill = 0;

    // Stop vehicle
    this->environment->stopAgent( this->private_vehicle );

    // Get out of vehicle
    this->private_vehicle->exit( this );
    foreach(Skill* s , this->private_vehicle->getSkills( VehicleSVMPollutionProduceSkill::staticMetaObject.className() )){
        VehiclePollutionProduceSkill* svm_skill = dynamic_cast<VehicleSVMPollutionProduceSkill*>( s );
        if( svm_skill && svm_skill->getPollutantType() == PollutionConstants::CC ){
            this->real_cc_pollution = this->real_cc_pollution + svm_skill->getProducedTotal();
        }
        if( svm_skill && svm_skill->getPollutantType() == PollutionConstants::DALY ){
            this->real_daly_pollution = this->real_daly_pollution + svm_skill->getProducedTotal();
        }
    }

    this->scenario_creator->global_chosen_mode.insert( this->remaining_legs.at(0).mode , this->scenario_creator->global_impact_cc.value( this->remaining_legs.at(0).mode , 0 ) + this->real_cc_pollution.number() );

    this->private_vehicle->deleteLater();
    this->private_vehicle = 0;
    this->getStyle()->icon_url = "http://cdn.flaticon.com/png/128/17447.png";

    this->remaining_legs.removeAt( 0 );
    this->status = 3;
    this->setNextTick();
}

// PUBLIC TRANSPORT

void GTPlatAgent::waitForPublicTransport(){
    // Do nothing, just wait 1 minute
    this->setNextTick( TimeUnit( 60 ) );
}

void GTPlatAgent::departingPublicTransport(PublicTransportVehicleDriver *driver, PublicTransportVehicle *vehicle, QList<PublicTransportStop *> remaining_stops){
    Q_UNUSED(driver)

    // Check if empty
    if( this->remaining_legs.isEmpty() ){
        vehicle->exit( this );
        this->getStyle()->icon_url = "http://cdn.flaticon.com/png/128/17447.png";
        this->disconnect( dynamic_cast<PublicTransportStop*>( QObject::sender() ) , &PublicTransportStop::departingPublicTransportSignal , this , &GTPlatAgent::departingPublicTransport );
        this->status = 3;
        this->setNextTick();
        return;
    }

    // Check if it goes by the stop we need to go
    GTPlatItinerary::Leg leg = this->remaining_legs.at(0);

    // If we have found our public transport vehicle, try to enter
    if( remaining_stops.contains( leg.to_stop ) && vehicle->enter( this ) ){

        // Disconnect from departing stop and connect to vehicle messages
        this->disconnect( leg.from_stop , &PublicTransportStop::departingPublicTransportSignal , this , &GTPlatAgent::departingPublicTransport );
        this->connect( vehicle , &PublicTransportVehicle::reachedStopSignal , this , &GTPlatAgent::reachedStop );
        return;
    }

    // Else continue waiting in stop

}

void GTPlatAgent::reachedStop(PublicTransportVehicleDriver *driver, PublicTransportVehicle *vehicle, PublicTransportStop *reached_stop){
    Q_UNUSED(driver)

    // Check if empty
    if( this->remaining_legs.isEmpty() ){
        vehicle->exit( this );
        this->getStyle()->icon_url = "http://cdn.flaticon.com/png/128/17447.png";
        this->disconnect( vehicle , &PublicTransportVehicle::reachedStopSignal , this , &GTPlatAgent::reachedStop );
        this->status = 3;
        this->setNextTick();
        return;
    }

    GTPlatItinerary::Leg leg = this->remaining_legs.at(0);
    PublicTransportStop* to_stop = leg.to_stop;

    // See if the reached stop is where we want to hop off
    if( reached_stop == to_stop ){
        // Get out of vehicle
        vehicle->exit( this );
        this->setGeometry( to_stop->getGeometry()->createCopy() );
        this->getStyle()->icon_url = "http://cdn.flaticon.com/png/128/17447.png";
        //emit this->getEnvironment()->interfaceSignal( this , "UPDATE" );

        this->disconnect( vehicle , &PublicTransportVehicle::reachedStopSignal , this , &GTPlatAgent::reachedStop );

        Debugger::critical( this , Q_FUNC_INFO , QString("YO ME BAJO %1").arg( driver->objectName() ) );

        this->remaining_legs.removeAt(0);
        this->status = 3;
        this->setNextTick();
        return;
    }

    // Else continue waiting inside vehicle
}

void GTPlatAgent::routeFinished(){

    // Stop myself
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    move_skill->brakeToStop();

    // PRINT LOG
    this->printLog( "FINISHED" );

    // Finish
    this->environment->stopAgent( this );
    //QTimer::singleShot( 1 , this , &Agent::deleteLater );
}

QNetworkReply* GTPlatAgent::requestGTALGItinerary( APIDriver* api , GSSCoordinate from, GSSCoordinate to, QString option ){
    QString url = "http://157.158.130.57:8081/gtalg/routers/biscay/plan?";

    url += QString( "&fromPlace=%1,%2" ).arg( from.getY() ).arg( from.getX() );
    url += QString( "&toPlace=%1,%2" ).arg( to.getY() ).arg( to.getX() );
    url += QString( "&time=10:00am&date=03-22-2017" );
    url += QString( "&mode=%1" ).arg( option == "TRANSIT" ? "TRANSIT,WALK" : option );
    url += QString( "&maxWalkDistance=3000&maxBikeDistance=10000&maxElectricCarDistance=112654" );
    url += QString( "&weightOptimization=QUICKER" );

    QMap<QString , QString> headers;
    headers.insert("Accept" , "application/json, text/javascript, */*; q=0.01");

    return api->GET( QUrl( url ) , headers );
}

void GTPlatAgent::printLog( QString filename ){

    // PRINT LOG
    if( this->chosen_itinerary ){
        Logger::log(
                    QString("%1;%2;%3;%4;%5;%6;%7;%8;%9;%10;%11;%12;%13;%14;%15;%16")
                    .arg( this->getId() )
                    .arg( QString("%1,%2").arg( this->departure_place.getX() ).arg( this->departure_place.getY() ) )
                    .arg( QString("%1,%2").arg( this->destination_place.getX() ).arg( this->destination_place.getY() ) )
                    .arg( this->profile->options.join(',') )
                    .arg( this->chosen_itinerary->expected_modes.isEmpty() ? "-" : this->chosen_itinerary->expected_modes.join(',') )
                    .arg( this->start_time.toString( "hh:mm:ss" ) )
                    .arg( this->chosen_itinerary->getExpectedDuration().number() )
                    .arg( this->chosen_itinerary->getExpectedLength().number() )
                    .arg( this->chosen_itinerary->expected_price.number() )
                    .arg( this->chosen_itinerary->getExpectedPollutionCC() )
                    .arg( this->chosen_itinerary->getExpectedPollutionDALY() )
                    .arg( this->start_time.secsTo( this->environment->getTimeEnvironment()->getCurrentDateTime() ) )
                    .arg( this->real_length.number() )
                    .arg( this->real_price.number() )
                    .arg( this->real_cc_pollution.number() )
                    .arg( this->real_daly_pollution.number() )
                    , filename );
    }

}
