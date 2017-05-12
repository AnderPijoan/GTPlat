#include "PollutionProduceSkill.h"

PollutionProduceSkill::PollutionProduceSkill(PollutionConstants::pollutant_types pollutant , KUnitConversion::UnitId production_unit , Agent *skilled_agent) : ProduceSkill( production_unit , skilled_agent ){
    this->pollutant = pollutant;
}

/**********************************************************************
 GETTERS
**********************************************************************/

PollutionConstants::pollutant_types PollutionProduceSkill::getPollutantType(){
    return this->pollutant;
}
