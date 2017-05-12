#include "VehicleDriver.h"

#include "geos/geom/CoordinateSequenceFactory.h"

#include "environment/Environment.h"
#include "skills/route/WalkPathRoutingSkill.h"
#include "skills/view/ViewRouteAheadSkill.h"

VehicleDriver::VehicleDriver(Vehicle* vehicle , PersonConstants::gender gender , GSSGeometry* geom , QObject* parent ) : Person( gender , geom , parent ){
    this->vehicle = vehicle;
    this->addSkill( new DriveSkill(this) );
    this->addSkill( new TransportLineRoutingSkill( this->vehicle , this ) );
    this->addSkill( new ViewRouteAheadSkill( this ) );
}

VehicleDriver::~VehicleDriver(){
}

QJsonObject VehicleDriver::toJSON(){
    QJsonObject agent_json = Person::toJSON();
    QJsonArray attributes = agent_json["attributes"].toArray();
    agent_json.insert("attributes" , attributes);
    return agent_json;
}

/**********************************************************************
 GETTERS
**********************************************************************/

Vehicle* VehicleDriver::getVehicle(){
    return this->vehicle;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void VehicleDriver::setDestination(GSSCoordinate destination){
    this->destination = destination;
}

void VehicleDriver::setVehicle(Vehicle* vehicle){
    this->vehicle = vehicle;
    Skill* skill = this->getSkill( TransportLineRoutingSkill::staticMetaObject.className() );
    this->removeSkill( skill );
    this->addSkill( new TransportLineRoutingSkill( this->vehicle , this ) );
}

/**********************************************************************
 METHODS
**********************************************************************/


/**********************************************************************
 SLOTS
**********************************************************************/

void VehicleDriver::behave(){

    switch (status) {
    case 0:

        // Calculate route to vehicle
        this->calculateRouteToVehicle();
        break;

    case 1:
    case 5:

        // Walk untill arriving at vehicle
        this->followWalkRoute();
        break;

    case 2:

        // Get inside vehicle and calculate route to vehicle park
        this->getInsideVehicleAndDrive();
        break;

    case 3:

        // Go driving to vehicle park
        this->followDriveRoute();
        break;

    case 4:

        // Park vehicle and calculate walking route to destination
        this->getOutsideVehicleAndWalk();
        break;

    case 6:
        this->routeFinished();
        break;
    }

}

void VehicleDriver::calculateRouteToVehicle(){

    // Calculate walking route to vehicle
    WalkPathRoutingSkill* walk_skill = dynamic_cast<WalkPathRoutingSkill*>( this->getSkill( WalkPathRoutingSkill::staticMetaObject.className() ) );
    if( !walk_skill ){
        Debugger::info( this , Q_FUNC_INFO , "Agent doesnt know how to route. Finishing agent" );
        this->environment->stopAgent( this );
        return;
    }

    // Calculate walking route
    walk_skill->calculateRoute( this->getRepresentativeCoordinate() , this->vehicle->getRepresentativeCoordinate() );
    status++;
    this->setNextTick();
}

void VehicleDriver::followWalkRoute(){

    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    if( !move_skill ){
        Debugger::info( this , Q_FUNC_INFO , "Agent cant' move" );
        this->environment->stopAgent( this );
        return;
    }

    // Go to next edge
    WalkPathRoutingSkill* walk_skill = dynamic_cast<WalkPathRoutingSkill*>( this->getSkill( WalkPathRoutingSkill::staticMetaObject.className() ) );
    WalkPath* following_walk_path = walk_skill->getFollowingEdge();
    if( !following_walk_path ){

        // Finished routing
        if( walk_skill->getCurrentEdge() ){ walk_skill->getCurrentEdge()->exit( this ); }
        move_skill->brakeToStop();
        status++;
        this->setNextTick();

    } else {

        // Walk to the max speed;
        move_skill->accelerate( 1 );

        // Check following_walk_path edge
        if( following_walk_path->isPermanentlyClosed() || !following_walk_path->isAllowed( this ) ){
            Debugger::info( this , Q_FUNC_INFO , "Tried to enter a closed or not allowed WalkPath" );
            this->environment->stopAgent( this );
            return;
        }

        if( following_walk_path->isTemporarilyClosed() ){
            // Could not enter, retry in 1 second
            this->setNextTick( TimeUnit(1) );
            return;
        }

        // Exit previous
        if( walk_skill->getCurrentEdge() ){ walk_skill->getCurrentEdge()->exit( this ); }

        // Enter edge and point to next
        following_walk_path->enter( this );
        walk_skill->pointToFollowingEdge();

        this->setGeometry( environment->getPhysicalEnvironment()->getGeometryController()->createPoint( walk_skill->getCurrentEdge()->getToNode()->getCoordinate() ) );

        // Calculate when we will arrive at edge end
        TimeUnit seconds = TimeUnit( walk_skill->getCurrentEdge()->getLength().number() / move_skill->getCurrentSpeed().number() );
        // Set next tick time after seconds
        this->setNextTick( seconds );
    }
}

void VehicleDriver::getInsideVehicleAndDrive(){

    // Stop walking
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    move_skill->brakeToStop();

    // Try to get inside vehicle
    if( !this->vehicle->enter( this ) ){
        Debugger::info( this , Q_FUNC_INFO , "Could not enter vehicle" );
        this->environment->stopAgent( this );
        return;
    }

    // Start vehicle
    this->environment->runAgent( this->vehicle );

    // Vehicles icon
    this->getStyle()->icon_url = this->vehicle->getStyle()->icon_url;

    // Calculate vehicle route
    TransportLineRoutingSkill* vehicle_routing_skill = dynamic_cast<TransportLineRoutingSkill*>( this->getSkill( TransportLineRoutingSkill::staticMetaObject.className() ) );
    vehicle_routing_skill->calculateRoute( this->getVehicle()->getRepresentativeCoordinate() , this->destination );
    status++;
    this->setNextTick();
}

void VehicleDriver::followDriveRoute(){

    //emit this->environment->pushToInterfaceSignal( this->toMiniJSON() , "UPDATE" );

    MoveSkill* vehicle_move_skill = dynamic_cast<MoveSkill*>( this->vehicle->getSkill( MoveSkill::staticMetaObject.className() ) );

    // Go to next edge
    TransportLineRoutingSkill* transport_skill = dynamic_cast<TransportLineRoutingSkill*>( this->getSkill( TransportLineRoutingSkill::staticMetaObject.className() ) );
    TransportLine* following_transport_line = transport_skill->getFollowingEdge();
    if( !following_transport_line ){

        // Finished routing
        if( transport_skill->getCurrentEdge() ){ transport_skill->getCurrentEdge()->exit( this->vehicle ); }
        vehicle_move_skill->brakeToStop();
        status++;
        this->setNextTick();

    } else {

        // Check following_walk_path edge
        if( following_transport_line->isPermanentlyClosed() || !following_transport_line->isAllowed( this->vehicle ) ){
            Debugger::info( this , Q_FUNC_INFO , "Tried to enter a closed or not allowed TransportLine" );
            this->environment->stopAgent( this->vehicle );
            this->environment->stopAgent( this );
            return;
        }

        // See if we have free road
        ViewRouteAheadSkill* view_skill = dynamic_cast<ViewRouteAheadSkill*>( this->getSkill( ViewRouteAheadSkill::staticMetaObject.className() ) );
        view_skill->updateViewingRoads( transport_skill->getRemainingRoute() );

        // We have currently no free distance to go on
        if( following_transport_line->isFull() && !following_transport_line->isAgentInside( this->vehicle ) ){
            Debugger::info( this , Q_FUNC_INFO , "Road is full, waiting for it to be free");
            vehicle_move_skill->brakeToStop();
            this->setNextTick( TimeUnit(2) );
            return;
        }

        // Exit previous
        if( transport_skill->getCurrentEdge() ){ transport_skill->getCurrentEdge()->exit( this->vehicle ); }

        // Enter edge and point to next
        following_transport_line->enter( this->vehicle );
        this->vehicle->setTravellingThrough( following_transport_line , view_skill->getTrafficLevel() );
        transport_skill->pointToFollowingEdge();

        // Decide drive speed according to traffic and free distance
        DriveSkill* drive_skill = dynamic_cast<DriveSkill*>( this->getSkill( DriveSkill::staticMetaObject.className() ) );
        vehicle_move_skill->brake( drive_skill->calculateBrakeForce( vehicle_move_skill->getCurrentSpeed() , transport_skill->getCurrentEdge()->getMaxSpeed() , view_skill->getFreeDistance() ) );
        vehicle_move_skill->accelerate( drive_skill->calculateAccelerateForce( vehicle_move_skill->getCurrentSpeed() , transport_skill->getCurrentEdge()->getMaxSpeed() , view_skill->getFreeDistance() ) );

        this->vehicle->setGeometry( transport_skill->getCurrentEdge()->getGeometry()->createCopy() );
        //emit this->environment->pushToInterfaceSignal( this->vehicle->toMiniJSON() , "UPDATE" );

        // Calculate when we will arrive at edge end
        TimeUnit seconds = TimeUnit( transport_skill->getCurrentEdge()->getLength().number() / vehicle_move_skill->getCurrentSpeed().number() );
        // Set next tick time after seconds
        this->setNextTick( seconds );
    }
}


void VehicleDriver::getOutsideVehicleAndWalk(){

    // Stop vehicle
    MoveSkill* vehicle_move_skill = dynamic_cast<MoveSkill*>( this->vehicle->getSkill( MoveSkill::staticMetaObject.className() ) );
    vehicle_move_skill->brakeToStop();

    // Stop vehicle
    this->environment->stopAgent( this->vehicle );

    // Get out of vehicle
    this->vehicle->exit( this );

    // Back to original icon
    this->getStyle()->icon_url = "http://cdn.flaticon.com/png/128/17447.png";

    // Calculate walking route to destination
    WalkPathRoutingSkill* walk_skill = dynamic_cast<WalkPathRoutingSkill*>( this->getSkill( WalkPathRoutingSkill::staticMetaObject.className() ) );
    if( !walk_skill ){
        Debugger::info( this , Q_FUNC_INFO , "Agent doesnt know how to route. Finishing agent" );
        this->environment->stopAgent( this );
        return;
    }

    // Calculate walking route
    walk_skill->calculateRoute( this->getRepresentativeCoordinate() , this->destination );
    status++;
    this->setNextTick();
}


void VehicleDriver::routeFinished(){

    // Stop myself
    MoveSkill* move_skill = dynamic_cast<MoveSkill*>( this->getSkill( MoveSkill::staticMetaObject.className() ) );
    move_skill->brakeToStop();

    // Finish
    this->environment->stopAgent( this );
}

