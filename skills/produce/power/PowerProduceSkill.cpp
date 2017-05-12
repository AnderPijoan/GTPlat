#include "PowerProduceSkill.h"

PowerProduceSkill::PowerProduceSkill(KUnitConversion::UnitId production_unit , Agent *skilled_agent) : ProduceSkill( production_unit , skilled_agent){
}

/**********************************************************************
 METHODS
**********************************************************************/

/**********************************************************************
 METHODS
**********************************************************************/

BaseUnit PowerProduceSkill::produce(){
    return PowerUnit( 0 );
}
