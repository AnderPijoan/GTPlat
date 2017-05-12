#include "ProduceSkill.h"

ProduceSkill::ProduceSkill(KUnitConversion::UnitId production_unit , Agent *skilled_agent) : Skill(skilled_agent) , production_unit(production_unit) {
    this->produced_total = BaseUnit( 0 , this->production_unit );
}

/**********************************************************************
 GETTERS
**********************************************************************/

KUnitConversion::UnitId ProduceSkill::getUnit() const{
    return this->production_unit;
}

BaseUnit ProduceSkill::getProducedTotal() const{
    return this->produced_total;
}

/**********************************************************************
 METHODS
**********************************************************************/

BaseUnit ProduceSkill::produce(){
    this->produced_total = this->produced_total + BaseUnit( 0 , this->production_unit );
    return this->produced_total;
}
