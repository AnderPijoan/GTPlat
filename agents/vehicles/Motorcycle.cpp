#include "Motorcycle.h"

Motorcycle::Motorcycle(GSSGeometry* geom , QObject* parent) : Vehicle( geom , parent ){
    this->getStyle()->border_weight = 8;
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/226/226577.png";
    this->usage_cost = 0.29 / 1000;

    foreach(Skill* s , this->getSkills(MoveSkill::staticMetaObject.className())){
        dynamic_cast<MoveSkill*>(s)->setMaxSpeed( SpeedUnit( 100 + ( qrand() % 50) , KilometerPerHour ) ); // From 100km to 150/h
    }
    foreach(Skill* s , this->getSkills(AgentStoreSkill::staticMetaObject.className())){
        dynamic_cast<AgentStoreSkill*>(s)->setMaxCapacity( 2 );
    }

}

