#include "Parking.h"

#include "skills/store/AgentStoreSkill.h"

Parking::Parking(GSSGeometry *geom, QObject *parent) : CivilStructure( geom , parent ) , TransportNode( geom->getRepresentativeCoordinate() ){

    // Create one AgentStoreSkill with capacity = 1
    this->addSkill( new AgentStoreSkill(this) );

    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/262/262392.png";
}

/**********************************************************************
 GETTERS
**********************************************************************/

bool Parking::isFull(){
    bool full = true;
    foreach(Skill* s , this->getSkills( AgentStoreSkill::staticMetaObject.className() ) ){
        full &= dynamic_cast<AgentStoreSkill*>( s )->isFull();
    }
    return full;
}
