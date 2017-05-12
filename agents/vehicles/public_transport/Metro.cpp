#include "Metro.h"

Metro::Metro(GSSGeometry* geom , QObject* parent) : PublicTransportVehicle( geom , parent ){
    this->getStyle()->border_weight = 20;
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/171/171248.png";

    foreach(Skill* s , this->getSkills(MoveSkill::staticMetaObject.className())){
        dynamic_cast<MoveSkill*>(s)->setMaxSpeed( SpeedUnit( 70 + ( qrand() % 30) , KilometerPerHour ) ); //From 70 to 100km/h
    }
    foreach(Skill* s , this->getSkills(AgentStoreSkill::staticMetaObject.className())){
        dynamic_cast<AgentStoreSkill*>(s)->setMaxCapacity( 200 );
    }
}

