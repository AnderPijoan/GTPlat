#include "VehicleRental.h"

#include "skills/store/AgentStoreSkill.h"

VehicleRental::VehicleRental(GSSGeometry *geom, QObject *parent) : CivilStructure( geom , parent ){

    // Set vehicle capacity to 20
    AgentStoreSkill* store = new AgentStoreSkill( this );
    store->setMaxCapacity( 20 );
    this->addSkill( store );
}

/**********************************************************************
 GETTERS
**********************************************************************/

bool VehicleRental::isFull(){
    bool f = true;
    foreach( Skill* s , this->getSkills( AgentStoreSkill::staticMetaObject.className() ) ){
        f &= dynamic_cast<AgentStoreSkill*>( s )->isFull();
    }
    return f;
}

bool VehicleRental::isEmpty(){
    bool e = true;
    foreach( Skill* s , this->getSkills( AgentStoreSkill::staticMetaObject.className() ) ){
        e &= dynamic_cast<AgentStoreSkill*>( s )->isEmpty();
    }
    return e;
}

int VehicleRental::countVehicles(){
    int c = 0;
    foreach( Skill* s , this->getSkills( AgentStoreSkill::staticMetaObject.className() ) ){
        c += dynamic_cast<AgentStoreSkill*>( s )->countAgentsInside();
    }
    return c;
}

/**********************************************************************
 SETTERS
**********************************************************************/

Vehicle* VehicleRental::rentVehicle(){

    // Get the first available vehicle
    foreach(Skill* s , this->getSkills( AgentStoreSkill::staticMetaObject.className() ) ){
        foreach( Agent* a , dynamic_cast<AgentStoreSkill*>(s)->getAgentsInsideByClass( Vehicle::staticMetaObject.className() ) ){
            // Cast to vehicle, as there might be more agents inside
            Vehicle* vehicle = dynamic_cast<Vehicle*>( a );
            if( vehicle ){
                bool able_to_retrieve = dynamic_cast<AgentStoreSkill*>(s)->exit( vehicle );
                if( able_to_retrieve) { return vehicle; }
            }
        }
    }
    return 0;
}

bool VehicleRental::returnVehicle(Vehicle *vehicle){

    // Check vehicle has been returned without damage :)

    // Check if rental is full
    if( this->isFull() ){
        return false;
    }

    foreach(Skill* s , this->getSkills( AgentStoreSkill::staticMetaObject.className() ) ){
        if( dynamic_cast<AgentStoreSkill*>( s )->enter( vehicle ) ){
            return true;
        }
    }
    return false;
}
