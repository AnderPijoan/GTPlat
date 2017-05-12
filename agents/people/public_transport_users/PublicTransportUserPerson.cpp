#include "PublicTransportUserPerson.h"
#include "environment/Environment.h"
#include "agents/transport_networks/roads/Road.h"
#include "agents/transport_networks/public_transport/PublicTransportStretch.h"
#include "skills/route/PublicTransportUseRoutingSkill.h"

PublicTransportUserPerson::PublicTransportUserPerson(PersonConstants::gender gender, GSSGeometry* geom, QObject* parent ) : Person( gender , geom, parent ){
    this->addSkill( new PublicTransportUseRoutingSkill(this) );
}

QJsonObject PublicTransportUserPerson::toJSON(){
    QJsonObject agent_json = Person::toJSON();
    QJsonArray attributes = agent_json["attributes"].toArray();
    agent_json.insert("attributes" , attributes);
    return agent_json;
}


/**********************************************************************
 GETTERS
**********************************************************************/

QDateTime PublicTransportUserPerson::getDeparture(){
    return this->departure;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void PublicTransportUserPerson::setDeparture(QDateTime departure){
    this->departure = departure;
}

void PublicTransportUserPerson::setDestination( GSSCoordinate destination ){
    this->destination = destination;
}

/**********************************************************************
 METHODS
**********************************************************************/




/**********************************************************************
 SLOTS
**********************************************************************/

/**
 * @brief VehicleDriver::start Calculate route and start driving from starting Node to ending Node
 */
void PublicTransportUserPerson::agentStarted(){

    // Inherited classes do no more
    if( this->metaObject()->className() != PublicTransportUserPerson::staticMetaObject.className() ){ return; }

    // Calculate the start time
    double seconds( this->getEnvironment()->getTimeEnvironment()->getCurrentDateTime().secsTo( this->departure ) );

    if( seconds < 0 ){
        this->environment->stopAgent( this );
        return;
    }

    // VehicleDriver agents, start moving to vehicle
    //QTimer::singleShot( this->getEnvironment()->getTimeEnvironment()->toEnvironmentMsecs( seconds ) , this , SLOT(planTrip()) );
}

void PublicTransportUserPerson::planTrip(){

    // If it is not running anymore, finish
    if( !this->isRunning() ){ return; }

    // Plan the trip
    PublicTransportUseRoutingSkill* trip_skill = dynamic_cast<PublicTransportUseRoutingSkill*>( this->getSkill( PublicTransportUseRoutingSkill::staticMetaObject.className() ) );
    this->connect( trip_skill , SIGNAL(routeCalculatedSignal()) , this , SLOT(calculateRouteToPublicTransportStop()) );
    //trip_skill->calculateRoute( this->getRepresentativeCoordinate() , this->destination );
}

void PublicTransportUserPerson::calculateRouteToPublicTransportStop(){

    PublicTransportUseRoutingSkill* put_skill = dynamic_cast<PublicTransportUseRoutingSkill*>( this->getSkill( PublicTransportUseRoutingSkill::staticMetaObject.className() ) );
    this->disconnect( put_skill , SIGNAL(routeCalculatedSignal()) , this , SLOT(calculateRouteToPublicTransportStop()) );

    // If it is not running anymore, finish
    if( !this->isRunning() ){ return; }

    this->remaining_stretchs = put_skill->getRemainingPublicTransportStretchs();

    if( this->remaining_stretchs.isEmpty() ){
        //QTimer::singleShot( 0 , this , SLOT(routeFinished()) );
        return;
    }

    // Calculate walking route to stop
    RoutingSkill* routing_skill = dynamic_cast<RoutingSkill*>( this->getSkill( RoutingSkill::staticMetaObject.className() ) );
    this->connect( routing_skill , SIGNAL(routeCalculatedSignal()) , this , SLOT(followWalkRoute()) );
    this->connect( routing_skill , SIGNAL(arrivedAtEdgeSignal()) , this , SLOT(followWalkRoute()) );
    this->connect( routing_skill , SIGNAL(couldNotReachEdgeSignal()) , this , SLOT(retryFollowWalkRoute()) );
    this->connect( routing_skill , SIGNAL(routeEndReachedSignal()) , this , SLOT(waitForPublicTransport()) );
    routing_skill->calculateRoute( this->getRepresentativeCoordinate() , this->remaining_stretchs.at(0)->getFromNode()->getCoordinate() );
}

void PublicTransportUserPerson::followWalkRoute(){

    // If it is not running anymore, finish
    if( !this->isRunning() ){ return; }

    // Walk to max speed
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    move_skill->accelerate( 1 );

    // Go to next edge
    RoutingSkill* routing_skill = dynamic_cast<RoutingSkill*>( this->getSkill( RoutingSkill::staticMetaObject.className() ) );
    routing_skill->getCurrentEdge();
}

void PublicTransportUserPerson::waitForPublicTransport(){

    // Disconnect from routing skill
    RoutingSkill* routing_skill = dynamic_cast<RoutingSkill*>( this->getSkill( RoutingSkill::staticMetaObject.className() ) );
    this->disconnect( routing_skill , SIGNAL(routeCalculatedSignal()) , this , SLOT(followWalkRoute()) );
    this->disconnect( routing_skill , SIGNAL(arrivedAtEdgeSignal()) , this , SLOT(followWalkRoute()) );
    this->disconnect( routing_skill , SIGNAL(couldNotReachEdgeSignal()) , this , SLOT(retryFollowWalkRoute()) );
    this->disconnect( routing_skill , SIGNAL(routeEndReachedSignal()) , this , SLOT(waitForPublicTransport()) );

    // If it is not running anymore, finish
    if( !this->isRunning() ){ return; }

    // Stop walking
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    move_skill->brakeToStop();

    // Try to get inside stop
    PublicTransportStop* stop = dynamic_cast<PublicTransportStop*>( this->remaining_stretchs.at(0)->getFromNode() );
    if( !stop->addWaitingPerson( this ) ){
        Debugger::info( this , Q_FUNC_INFO , "Public transport stop is full" );
        return;
    }

    // Listen messages from public transport stop
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/61/61298.png";
    if( this->getEnvironment() ){ emit this->getEnvironment()->pushToInterfaceSignal( this->toMiniJSON() , "UPDATE" ); }
    this->connect( stop , SIGNAL(departingPublicTransport(PublicTransportVehicleDriver*,PublicTransportVehicle*,QList<PublicTransportStop*>)) , this , SLOT(departingPublicTransport(PublicTransportVehicleDriver*,PublicTransportVehicle*,QList<PublicTransportStop*>)) );
}

void PublicTransportUserPerson::departingPublicTransport(PublicTransportVehicleDriver *driver, PublicTransportVehicle *vehicle, QList<PublicTransportStop *> passing_by_stops){
    Q_UNUSED(driver)

    QMutexLocker(&this->mutex);

    // Check if it goes by the stop we need to go
    PublicTransportStop* from_stop = dynamic_cast<PublicTransportStop*>( this->remaining_stretchs.at(0)->getFromNode() );
    PublicTransportStop* to_stop = dynamic_cast<PublicTransportStop*>( this->remaining_stretchs.at(0)->getToNode() );

    // If we have found our public transport vehicle, try to enter
    if( passing_by_stops.contains( to_stop ) && vehicle->enter( this ) ){

        // Disconnect from departing stop and connect to vehicle messages
        this->disconnect( from_stop , SIGNAL(departingPublicTransport(PublicTransportVehicleDriver*,PublicTransportVehicle*,QList<PublicTransportStop*>)) , this , SLOT(departingPublicTransport(PublicTransportVehicleDriver*,PublicTransportVehicle*,QList<PublicTransportStop*>)) );
        this->connect( vehicle , SIGNAL(reachedStopSignal(PublicTransportVehicleDriver*,PublicTransportVehicle*,PublicTransportStop*)) , this , SLOT(reachedStop(PublicTransportVehicleDriver*,PublicTransportVehicle*,PublicTransportStop*)) );
        return;
    }

    // Else continue waiting
}


void PublicTransportUserPerson::reachedStop(PublicTransportVehicleDriver *driver, PublicTransportVehicle *vehicle, PublicTransportStop* stop){
    Q_UNUSED(driver)

    PublicTransportStop* to_stop = dynamic_cast<PublicTransportStop*>( this->remaining_stretchs.at(0)->getToNode() );

    // See if the reached stop is where we want to hop off
    if( stop == to_stop ){
        // Get out of vehicle
        vehicle->exit( this );
        this->setGeometry( to_stop->getGeometry()->createCopy() );
        this->getStyle()->icon_url = "http://cdn.flaticon.com/png/128/17447.png";
        if( this->getEnvironment() ){ emit this->getEnvironment()->pushToInterfaceSignal( this->toMiniJSON() , "UPDATE" ); }

        this->remaining_stretchs.removeAt(0);

        //QTimer::singleShot( 0 , this , SLOT(calculateRouteToPublicTransportStop()) );
        return;
    }

    // Else continue waiting
}


void PublicTransportUserPerson::routeFinished(){

    // Stop myself
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    move_skill->brakeToStop();

    // Finish
    //QTimer::singleShot( 0 , this , SLOT( agentEnded()) );
}

void PublicTransportUserPerson::retryFollowWalkRoute(){

    // Stop myself
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    move_skill->brakeToStop();

    // Retry in 1 second to follow route
    //QTimer::singleShot( this->getEnvironment()->getTimeEnvironment()->toEnvironmentMsecs( 1 ) , this , SLOT( followWalkRoute()) );
}
