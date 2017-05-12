#include "BicycleRental.h"

#include "skills/store/AgentStoreSkill.h"

BicycleRental::BicycleRental(GSSGeometry* geom , QObject *parent) : VehicleRental(geom , parent){
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/34/34665.png";
}

/**********************************************************************
 SETTERS
**********************************************************************/

Bicycle* BicycleRental::rentVehicle(){

    // Get the first available vehicle
    foreach(Skill* s , this->getSkills( AgentStoreSkill::staticMetaObject.className() ) ){
        foreach( Agent* a , dynamic_cast<AgentStoreSkill*>(s)->getAgentsInsideByClass( Bicycle::staticMetaObject.className() ) ){
            // Cast to Bike, as there might be more agents inside
            Bicycle* vehicle = dynamic_cast<Bicycle*>( a );
            if( vehicle ){
                bool able_to_retrieve = dynamic_cast<AgentStoreSkill*>(s)->exit( vehicle );
                if( able_to_retrieve) { return vehicle; }
            }
        }
    }
    return 0;
}

bool BicycleRental::returnVehicle(Bicycle *bike){
    return VehicleRental::returnVehicle( bike );
}
