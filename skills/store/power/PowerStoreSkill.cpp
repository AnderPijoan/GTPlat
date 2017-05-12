#include "PowerStoreSkill.h"

PowerStoreSkill::PowerStoreSkill(Agent *skilled_agent) : Skill( skilled_agent ){
    this->amount_inside = 0;
}

/**********************************************************************
 GETTERS
**********************************************************************/

bool PowerStoreSkill::isFullyLoaded(){
    return this->amount_inside >= this->max_capacity;
}

double PowerStoreSkill::getAmountInside(){
    return this->amount_inside;
}

double PowerStoreSkill::getMaxCapacity(){
    return this->max_capacity;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void PowerStoreSkill::setMaxCapacity(double capacity){
    this->max_capacity = capacity;
}
