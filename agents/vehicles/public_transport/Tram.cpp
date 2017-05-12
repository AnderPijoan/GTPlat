#include "Tram.h"

Tram::Tram(GSSGeometry* geom , QObject *parent) : PublicTransportVehicle(geom , parent){
    this->getStyle()->border_weight = 20;
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/171/171250.png";

    foreach(Skill* s , this->getSkills(MoveSkill::staticMetaObject.className())){
        dynamic_cast<MoveSkill*>(s)->setMaxSpeed( SpeedUnit( 70 + ( qrand() % 20) , KilometerPerHour ) ); //From 70 to 90km/h
    }
    foreach(Skill* s , this->getSkills(AgentStoreSkill::staticMetaObject.className())){
        dynamic_cast<AgentStoreSkill*>(s)->setMaxCapacity( 200 );
    }
}

