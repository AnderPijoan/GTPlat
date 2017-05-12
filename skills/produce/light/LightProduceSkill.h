#ifndef LIGHTPRODUCESKILL_H
#define LIGHTPRODUCESKILL_H

#include "skills/produce/ProduceSkill.h"

class LightProduceSkill : public ProduceSkill
{
    Q_OBJECT

public:
    LightProduceSkill(KUnitConversion::UnitId production_unit = KUnitConversion::Hertz , Agent* skilled_agent = 0);

    // GETTERS
    QString getTechnology();
    double getLuminousFlux();

    // METHODS
    virtual double produce( PowerUnit supplied_power , PowerUnit& consumed_power );

private:
    double luminous_flux; // In lumens - Maximum light output which can be provided by the lamp
    QString technology; // LED, LPS, HPS - Technology used by the lamp
};

#endif // LIGHTPRODUCESKILL_H
