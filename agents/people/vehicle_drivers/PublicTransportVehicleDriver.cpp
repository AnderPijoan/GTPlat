#include "PublicTransportVehicleDriver.h"
#include "environment/Environment.h"

PublicTransportVehicleDriver::PublicTransportVehicleDriver(PublicTransportVehicle *vehicle, PersonConstants::gender gender, GSSGeometry* geom , QObject* parent) : ItineraryVehicleDriver( vehicle , gender , geom , parent ){
}

QJsonObject PublicTransportVehicleDriver::toJSON(){
    QJsonObject agent_json = VehicleDriver::toJSON();
    agent_json.insert( "driver-agency", CustomJsonExporter::format( this->agency_name )  );
    agent_json.insert( "driver-route", CustomJsonExporter::format( this->route_name )  );
    return agent_json;
}

/**********************************************************************
 GETTERS
**********************************************************************/

QString PublicTransportVehicleDriver::getAgencyName(){
    return this->agency_name;
}

QString PublicTransportVehicleDriver::getRouteName(){
    return this->route_name;
}

QString PublicTransportVehicleDriver::getRouteId(){
    return this->route_id;
}

QString PublicTransportVehicleDriver::getRouteVehicleType(){
    return this->vehicle_type;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void PublicTransportVehicleDriver::setVehicle(PublicTransportVehicle *vehicle){
    ItineraryVehicleDriver::setVehicle( vehicle );
}

void PublicTransportVehicleDriver::setAgencyName(QString agency_name){
    this->agency_name = agency_name;
}

void PublicTransportVehicleDriver::setRouteName(QString route_name){
    this->route_name = route_name;
}

void PublicTransportVehicleDriver::setRouteId(QString route_id){
    this->route_id = route_id;
}

void PublicTransportVehicleDriver::setRouteVehicleType(QString vehicle_type){
    this->vehicle_type = vehicle_type;
}

/**********************************************************************
 SLOTS
**********************************************************************/


void PublicTransportVehicleDriver::stopAtItineraryPoint(){

    PublicTransportStop* pt_stop = dynamic_cast<PublicTransportStop*>( this->remaining_stops.at(0).stop );

    if( pt_stop ){

        PublicTransportVehicle* pt_vehicle = dynamic_cast<PublicTransportVehicle*>( this->vehicle );
        if( pt_vehicle ){
            emit pt_vehicle->reachedStopSignal( this , pt_vehicle , pt_stop );
        }
    }

    ItineraryVehicleDriver::stopAtItineraryPoint();
}

void PublicTransportVehicleDriver::departFromItineraryPoint(){

    PublicTransportStop* pt_stop = dynamic_cast<PublicTransportStop*>( this->remaining_stops.at(0).stop );
    if( pt_stop ){

        // Get the remaining stops
        QList<PublicTransportStop*> travelling_to;
        foreach( ScheduledStop s , this->remaining_stops ){
            PublicTransportStop* ss = dynamic_cast<PublicTransportStop*>( s.stop );
            if( ss ){
                travelling_to.append( ss );
            }
        }

        PublicTransportVehicle* pt_vehicle = dynamic_cast<PublicTransportVehicle*>( this->vehicle );
        if( pt_vehicle ){
            emit pt_stop->departingPublicTransportSignal( this , pt_vehicle , travelling_to );
        }
    }

    ItineraryVehicleDriver::departFromItineraryPoint();
}

