#include "SolarPowerProduceSkill.h"

#include "environment/Environment.h"

SolarPowerProduceSkill::SolarPowerProduceSkill( KUnitConversion::UnitId production_unit, Agent *skilled_agent) : PowerProduceSkill( production_unit , skilled_agent ){
}

/**********************************************************************
 METHODS
**********************************************************************/

BaseUnit SolarPowerProduceSkill::produce(){

   // Get solar irradiation
   //double sun_irradiation = this->skilled_agent->getEnvironment()->getClimateEnvironment()->getSunIrradiation( this->skilled_agent->getRepresentativeCoordinate() );

   // Transform sun into power
   PowerUnit power = PowerUnit( 10 * (qrand() % 10) );// TODO

   // Return produced solar power
   return power;
}
