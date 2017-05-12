#include "Ferry.h"

Ferry::Ferry(GSSGeometry* geom , QObject *parent) : PublicTransportVehicle(geom , parent){
    this->getStyle()->border_weight = 20;
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/198/198367.png";

    foreach(Skill* s , this->getSkills(MoveSkill::staticMetaObject.className())){
        dynamic_cast<MoveSkill*>(s)->setMaxSpeed( SpeedUnit( 25 + qrand() % 10, KilometerPerHour ) ); //From 25 to 35km/h
    }
    foreach(Skill* s , this->getSkills(AgentStoreSkill::staticMetaObject.className())){
        dynamic_cast<AgentStoreSkill*>(s)->setMaxCapacity( 300 );
    }
}

