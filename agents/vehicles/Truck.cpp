#include "Truck.h"

Truck::Truck(GSSGeometry* geom , QObject* parent) : Vehicle(geom , parent){
    this->getStyle()->border_weight = 20;
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/171/171258.png";

    foreach(Skill* s , this->getSkills(MoveSkill::staticMetaObject.className())){
        dynamic_cast<MoveSkill*>(s)->setMaxSpeed( SpeedUnit( 80 + ( qrand() % 30) , KilometerPerHour ) ); //From 80 to 110km/h
    }
    foreach(Skill* s , this->getSkills(AgentStoreSkill::staticMetaObject.className())){
        dynamic_cast<AgentStoreSkill*>(s)->setMaxCapacity( 5 );
    }
}

