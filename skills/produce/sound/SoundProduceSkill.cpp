#include "SoundProduceSkill.h"

SoundProduceSkill::SoundProduceSkill(KUnitConversion::UnitId production_unit , Agent *skilled_agent) : ProduceSkill( production_unit , skilled_agent ){

}

/**********************************************************************
 METHODS
**********************************************************************/

FrequencyUnit SoundProduceSkill::produce( PowerUnit supplied_power , PowerUnit& consumed_power ){

    // Consume all supplied power
    consumed_power = supplied_power * 0.05;

    if( supplied_power <= PowerUnit( 0 ) ){
        return FrequencyUnit( 0 );
    }

    return FrequencyUnit( 20 );
}
