#include "Train.h"

Train::Train(GSSGeometry* geom , QObject *parent) : PublicTransportVehicle(geom , parent){
    this->getStyle()->border_weight = 20;
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/179/179591.png";

    foreach(Skill* s , this->getSkills(MoveSkill::staticMetaObject.className())){
        dynamic_cast<MoveSkill*>(s)->setMaxSpeed( SpeedUnit( 100 + ( qrand() % 50) , KilometerPerHour ) ); //From 100km to 150/h
    }
    foreach(Skill* s , this->getSkills(AgentStoreSkill::staticMetaObject.className())){
        dynamic_cast<AgentStoreSkill*>(s)->setMaxCapacity( 300 );
    }
}

