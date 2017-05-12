#include "Car.h"

Car::Car(GSSGeometry* geom , QObject* parent) : Vehicle( geom , parent ){
    this->getStyle()->border_weight = 10;
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/179/179519.png";
    this->usage_cost = 0.29 / 1000;

    foreach(Skill* s , this->getSkills(MoveSkill::staticMetaObject.className())){
        dynamic_cast<MoveSkill*>(s)->setMaxSpeed( SpeedUnit( 100 + ( qrand() % 50) , KilometerPerHour ) ); //From 100 to 150km/h
    }
    foreach(Skill* s , this->getSkills(AgentStoreSkill::staticMetaObject.className())){
        dynamic_cast<AgentStoreSkill*>(s)->setMaxCapacity( 5 );
    }
}

