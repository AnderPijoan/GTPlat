#include "Funicular.h"

Funicular::Funicular(GSSGeometry* geom , QObject *parent) : PublicTransportVehicle(geom , parent){
    this->getStyle()->border_weight = 20;
    this->getStyle()->icon_url = "http://image005.flaticon.com/8/png/128/8/8661.png";

    foreach(Skill* s , this->getSkills(MoveSkill::staticMetaObject.className())){
        dynamic_cast<MoveSkill*>(s)->setMaxSpeed( SpeedUnit( 25 , KilometerPerHour ) ); //25km/h
    }
    foreach(Skill* s , this->getSkills(AgentStoreSkill::staticMetaObject.className())){
        dynamic_cast<AgentStoreSkill*>(s)->setMaxCapacity( 70 );
    }
}

