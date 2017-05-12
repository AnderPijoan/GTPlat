#include "ItineraryVehicleDriver.h"

#include "environment/Environment.h"
#include "skills/route/RoutingSkill.h"
#include "skills/move/MoveSkill.h"

ItineraryVehicleDriver::ItineraryVehicleDriver(Vehicle *vehicle, PersonConstants::gender gender , GSSGeometry* geom , QObject* parent ) : VehicleDriver( vehicle , gender , geom , parent ){
}

/**********************************************************************
 GETTERS
**********************************************************************/

QList<ItineraryVehicleDriver::ScheduledStop> ItineraryVehicleDriver::getItinerary(){
    return this->itinerary;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void ItineraryVehicleDriver::setItinerary(QList<ItineraryVehicleDriver::ScheduledStop> itinerary){
    this->itinerary.clear();
    foreach(ItineraryVehicleDriver::ScheduledStop s , itinerary){
        if( s.stop && s.departure.isValid() && s.arrival.isValid() ){
            this->itinerary.append( s );
        }
    }
}


/**********************************************************************
 SLOTS
**********************************************************************/

void ItineraryVehicleDriver::behave(){

    switch (status) {
    case 0:

        // Calculate route to vehicle
        this->calculateRouteToVehicle();
        break;

    case 1:
    case 8:

        // Walk untill arriving at vehicle
        this->followWalkRoute();
        break;

    case 2:

        // Get inside vehicle and set itinerary
        this->getInsideVehicleAndDrive();
        break;

    case 3:

        // Calculate route to to next itinerary point
        this->calculateRouteToNextPoint();
        break;

    case 4:

        // Go driving to vehicle park
        this->followDriveRoute();
        emit this->environment->pushToInterfaceSignal( this->toMiniJSON() , "UPDATE" );
        break;

    case 5:

        // Arrived at itinerary point
        this->stopAtItineraryPoint();
        break;

    case 6:

        // Depart from itinerary point
        this->departFromItineraryPoint();
        break;

    case 7:
        // Park vehicle and calculate walking route to destination
        this->getOutsideVehicleAndWalk();
        break;

    case 9:
        this->routeFinished();
        break;
    }
}

void ItineraryVehicleDriver::getInsideVehicleAndDrive(){

    // Stop walking
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    move_skill->brakeToStop();

    // Try to get inside vehicle
    if( !this->vehicle->enter( this ) ){
        Debugger::warning( this , Q_FUNC_INFO , "Could not enter vehicle" );
        this->environment->stopAgent( this );
        return;
    }

    // Start vehicle
    this->environment->runAgent( this->vehicle );
    this->getStyle()->icon_url = this->vehicle->getStyle()->icon_url;

    // Set itinerary and start routing
    this->remaining_stops = this->itinerary;

    this->status++;
    this->setNextTick();
}

void ItineraryVehicleDriver::calculateRouteToNextPoint(){

    // Try to start vehicle, in case it was turned off
    if( !this->vehicle->isRunning() ){
        this->environment->runAgent( this->vehicle );
    }

    TransportLineRoutingSkill* vehicle_routing_skill = dynamic_cast<TransportLineRoutingSkill*>( this->getSkill( TransportLineRoutingSkill::staticMetaObject.className() ) );
    if( this->remaining_stops.isEmpty() ){

        vehicle_routing_skill->calculateRoute( this->vehicle->getRepresentativeCoordinate() , this->destination );
        status = 7;
        this->setNextTick();

    } else {

        // Calculate vehicle route to next itinerary point
        ItineraryVehicleDriver::ScheduledStop next_stop = this->remaining_stops.at(0);
        vehicle_routing_skill->calculateRoute( this->vehicle->getRepresentativeCoordinate() , next_stop.stop->getRepresentativeCoordinate() );
        status++;
        this->setNextTick();
    }
}

void ItineraryVehicleDriver::stopAtItineraryPoint(){

    // Stop vehicle
    MoveSkill* vehicle_move_skill = dynamic_cast<MoveSkill*>( this->vehicle->getSkill( MoveSkill::staticMetaObject.className() ) );
    vehicle_move_skill->brakeToStop();

    // Get the current stop
    ItineraryVehicleDriver::ScheduledStop scheduled_stop = this->remaining_stops.at(0);
    this->vehicle->setGeometry( scheduled_stop.stop->getGeometry()->createCentroid() );
    this->setGeometry( scheduled_stop.stop->getGeometry()->createCentroid() );

    // Calculate departure
    QDateTime departure = scheduled_stop.departure;
    TimeUnit seconds = TimeUnit( qMax( qint64(15) , this->getEnvironment()->getTimeEnvironment()->getCurrentDateTime().secsTo( departure ) ) ); // At least wait 15 seconds

    // If seconds is more than 5 minutes, turn off vehicle
    if( seconds > TimeUnit( 300 ) ){
        // Turn off vehicle
        this->environment->stopAgent( this->vehicle );
    }

    // Be ready to departure when at departure time
    status++;
    this->setNextTick( seconds );
}

void ItineraryVehicleDriver::departFromItineraryPoint(){

    // Pop last visited itinerary point
    this->remaining_stops.removeAt(0);

    // Repeat getInsideVehicleAndDrive();
    status = 3;
    this->setNextTick();
}
