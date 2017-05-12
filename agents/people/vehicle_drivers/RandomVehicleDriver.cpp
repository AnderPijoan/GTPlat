#include "RandomVehicleDriver.h"

#include "environment/Environment.h"
#include "skills/random_generators/RouletteWheelSkill.h"

RandomVehicleDriver::RandomVehicleDriver(Vehicle *vehicle, PersonConstants::gender gender) : VehicleDriver(vehicle , gender) {
    this->class_names.append( this->metaObject()->className() );
}

/**********************************************************************
 METHODS
**********************************************************************/

void RandomVehicleDriver::updateViewingRoads(){

    int occupied_roads_amount = 0;
    this->current_free_distance = this->stop_distance;
    double viewed_distance = 0;
    QList<RoadNetworkWay*> updated_viewing_roads;
    GraphEdge* last_road = this->moving_current_edge;

    // Iterate view_roads_list
    foreach(RoadNetworkWay* r , this->viewing_roads){
        updated_viewing_roads.append( r );
        viewed_distance += r->getLength();
        if( !r->canVehicleEnter( this->getVehicle() ) ){
            this->current_free_distance = qMin( this->current_free_distance , viewed_distance );
            occupied_roads_amount++;
        }
        last_road = r;
    }

    bool dead_end = false;

    // If we have view_distance to add some more roads
    while( viewed_distance < this->view_distance && !dead_end ){

        // Get all departing roads from last road in viewing_roads
        RouletteWheelSkill* roulette = new RouletteWheelSkill();
        foreach( GraphEdge* e , last_road->getEndNode()->getDepartingEdges() ){
            RoadNetworkWay* road = dynamic_cast<RoadNetworkWay*>( e );
            if( road && road->getAllowedVehicles().contains( this->getVehicle()->getVehicleType() ) ){

                if( updated_viewing_roads.contains( road ) ){ roulette->addElement( road , 0.2 ); } // Avoid repeating roads
                else if( road->canVehicleEnter( this->getVehicle() ) ){ roulette->addElement( road , 1 ); } // Avoid full roads
                else if( road->isClosed() ){ roulette->addElement( road , 1 ); } // Avoid closed roads
                else { roulette->addElement( road , 50 ); }
            }
        }

        // Check if reached dead end
        if( roulette->isEmpty() ){
            dead_end = true;
            break;
        }

        RoadNetworkWay* r = dynamic_cast<RoadNetworkWay*>( roulette->getElement() );
        updated_viewing_roads.append( r );
        viewed_distance += r->getLength();
        last_road = r;

        if( !r->canVehicleEnter( this->getVehicle() ) ){
            this->current_free_distance = qMin( this->current_free_distance , viewed_distance );
            occupied_roads_amount++;
        }

        roulette->deleteLater();
    }

    this->viewing_roads = updated_viewing_roads;
    this->current_traffic_level = this->viewing_roads.isEmpty() ? 1 :  1 - ( occupied_roads_amount / this->viewing_roads.size() );
}

/**********************************************************************
 SLOTS
**********************************************************************/

void RandomVehicleDriver::agentStarted(){
    Agent::agentStarted();

    if( !this->getVehicle() ){
        qDebug() << "[RandomVehicleDriver::agentStarted()] Agent has no vehicle.";
        this->getTimer()->singleShot( 0 , this, SLOT(agentEnded()) );
        return;
    }


    // Current GraphNode
    /*GraphNode* current = this->moving_from;
    if( this->moving_current_edge ){
        current = this->moving_current_edge->getStartNode();
    }

    // Reached dead end
    QList<GraphEdge*> departing_edges = current->getDepartingEdges();
    if( departing_edges.isEmpty() ){
        this->getTimer()->singleShot(0 , this , SLOT( movingEnded() ) );
        return;
    }

    // Check from the departing edges of the current_node, how many of them can we use
    QList<Road*> usable_roads;
    foreach( GraphEdge* e , departing_edges ){
        Road* road = dynamic_cast<Road*>( e );
        if( road && !road->isClosed() && road->vehicleTypeIsAllowed( this->getVehicle()->getVehicleType() ) ){
            usable_roads.append( road );
        }
    }

    // Check if there are any usable roads
    if( usable_roads.isEmpty() ){
        this->getTimer()->singleShot(0 , this , SLOT( movingEnded() ) );
        return;
    }

    // Get a random current_edge
    this->moving_current_edge = usable_roads.at( qrand() % usable_roads.size() );
    if( !this->moving_current_edge ){
        this->getTimer()->singleShot(0 , this , SLOT( movingEnded() ) );
        return;
    }

    // Fill the view_roads list now that we have a current_edge
    this->updateViewingRoads();

    this->getTimer()->singleShot(0 , this , SLOT( movingStarted() ) );*/
}


/**
 * @brief VehicleDriver::moveToNextRouteRoad Try to go to next route road
 */
void RandomVehicleDriver::moveToNextEdge(){

    // No vehicle
    if( !this->getVehicle() ){
        qWarning() << "[RandomVehicleDriver::moveToNextEdge] Driver has no vehicle.";
        this->getTimer()->singleShot(0 , this , SLOT( movingEnded() ) );
        return;
    }

    // Route finished
    if( this->moving_current_edge->getEndNode()->getCoordinate().equals( *this->moving_to->getGraphNodeCoordinate() ) ){
        this->getTimer()->singleShot( 0 , this, SLOT( movingEnded() ) );
        return;
    }


    // Otherwise start operations

    // Check if reached dead end
    if( this->viewing_roads.isEmpty() ){
        this->getTimer()->singleShot(0 , this , SLOT( movingEnded() ) );
        return;
    }

    RoadNetworkWay* next_road = this->viewing_roads.at(0);

    // If vehicle is not allowed, recalculate route
    if( !next_road->getAllowedVehicles().contains( this->getVehicle()->getVehicleType() ) ){
        this->getVehicle()->brakeToStop();
        this->getStyle()->border_color = this->getVehicle()->getStyle()->border_color = QColor("red");
        emit updateUISignal( this->getVehicle() );
        emit updateUISignal( this );
        qWarning() << "[RandomVehicleDriver::moveToNextEdge] Vehicle not allowed. Recalculating route.";
        this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds( 1 ) , this , SLOT( calculateRoute() ) );
        return;
    }

    // Try to enter next road, otherwise try again in 1 second
    if( next_road->isClosed() || !next_road->canVehicleEnter( this->getVehicle() ) ) {
        this->getVehicle()->brakeToStop();
        this->getStyle()->border_color = this->getVehicle()->getStyle()->border_color = QColor("red");
        emit updateUISignal( this->getVehicle()  );
        emit updateUISignal( this );
        this->getTimer()->singleShot( 0 , this , SLOT( vehicleCantEnterRoad() ) );
        return;
    }

    // Enter the road and remove it from viewing_roads
    next_road->enterVehicle( this->getVehicle() );
    this->viewing_roads.removeAt(0);

    // Check how many next route roads are occupied to know if there is
    // traffic and where the next stop is
    {

        this->updateViewingRoads();

        double free_distance_factor = this->stop_distance <= 0 ? 1 : this->current_free_distance / this->stop_distance;
        double speed_difference_factor = this->getVehicle()->getCurrentSpeed() - next_road->getMaxSpeed();

        // Set color
        this->getStyle()->border_color = this->getVehicle()->getStyle()->border_color = QColor( 255 - (255 * free_distance_factor), 255 * free_distance_factor, 128);

        // Accelerate or brake
        double f = free_distance_factor;
        this->getVehicle()->accelerate( f );
        //this->getVehicle()->brake( 1 - f );

    }

    // If speed turns out to be 0, try again in 1 second
    if( this->getVehicle()->getCurrentSpeed() < 0.01 ){
        this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds( 1 ) , this , SLOT(moveToNextEdge()) );
        return;
    }

    // If we have entered next road, exit previous road (current_edge) and update it
    if( this->moving_current_edge ){
        RoadNetworkWay* r = dynamic_cast<RoadNetworkWay*>( this->moving_current_edge );
        r->exitVehicle( this->getVehicle() );
    }
    this->moving_current_edge = next_road;

    // Tell vehicle we are travelling the new road (it will update the vehicles geometry)
    this->getVehicle()->travellingThrough( next_road , this->getCurrentTraffic() );
    this->setGeometry( this->getVehicle()->getGeometryCopy() );

    // UPDATE
    emit updateUISignal( this->getVehicle() );
    emit updateUISignal( this );

    // Calculate how long it will take to arrive at the next step
    double vehicle_speed = qMax( this->getVehicle()->getCurrentSpeed() , 1.0 ); // Avoid speed 0
    double seconds = this->moving_current_edge->getLength() / vehicle_speed;
    this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds( seconds ) , this , SLOT(moveToNextEdge()) );
}

void RandomVehicleDriver::vehicleCantEnterRoad(){

    // If traffic seems to be interrupted
    if( this->current_traffic_level <= 0.1 ){

        // TRY TO RECALCULATE ROUTE
        qWarning() << "[RandomVehicleDriver::cantEnterRoad] Traffic is interrupted, trying to recalculate route";
        this->getTimer()->singleShot(  TimeController::secondsToEnvironmentMseconds( 1 ) , this , SLOT( calculateRoute() ) );
        return;
    }

    // Try to enter again in 1 second
    this->getTimer()->singleShot(  TimeController::secondsToEnvironmentMseconds( 1 ) , this , SLOT(moveToNextEdge()) );
}
