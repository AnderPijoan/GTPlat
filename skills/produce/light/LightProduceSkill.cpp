#include "LightProduceSkill.h"

LightProduceSkill::LightProduceSkill(KUnitConversion::UnitId production_unit , Agent *skilled_agent) : ProduceSkill( production_unit , skilled_agent ){

}

/**********************************************************************
 GETTERS
**********************************************************************/

QString LightProduceSkill::getTechnology(){
    return this->technology;
}

double LightProduceSkill::getLuminousFlux(){
    return this->luminous_flux;
}

/**********************************************************************
 METHODS
**********************************************************************/


double LightProduceSkill::produce( PowerUnit supplied_power , PowerUnit& consumed_power ){

    // Consume all supplied power
    consumed_power = supplied_power;

    if( supplied_power <= PowerUnit( 0 ) ){
        return double( 0 );
    }

    return double( 1190 );
}
