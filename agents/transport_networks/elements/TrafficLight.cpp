#include "TrafficLight.h"
#include "environment/Environment.h"

TrafficLight::TrafficLight( GSSGeometry* geom , QObject* parent ) : Agent( geom , parent ){
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/190/190302.png";
}

TrafficLight::~TrafficLight(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

int TrafficLight::getGroupAmount(){
    return this->road_groups.size();
}

/**********************************************************************
 SETTERS
**********************************************************************/

void TrafficLight::addRoadGroup(TimeUnit crossable_duration, QList<TransportLine*> roads){
    TrafficLightGroup g;
    g.duration = crossable_duration;
    g.roads = roads;
    this->road_groups.append( g );
}

/**********************************************************************
 METHODS
**********************************************************************/

void TrafficLight::behave(){

    if( this->road_groups.isEmpty() ){
        return;
    }

    // Turn all traffic_lights red = roads not crossable
    foreach(TrafficLightGroup g , this->road_groups ){
        foreach(TransportLine* road , g.roads){
            road->setTemporarilyClosed( true ); // Close road
        }
    }

    this->current_crossable_group_index = (this->current_crossable_group_index+1) % this->road_groups.size();
    TrafficLightGroup g = this->road_groups.at( this->current_crossable_group_index );
    foreach( TransportLine* road , g.roads ){
        road->setTemporarilyClosed( false ); // Open road
    }

    //emit this->environment->interfaceSignal( this , "UPDATE" );
    this->setNextTick( g.duration );
}
