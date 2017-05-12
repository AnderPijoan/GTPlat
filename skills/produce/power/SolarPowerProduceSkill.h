#ifndef SOLARPOWERPRODUCESKILL_H
#define SOLARPOWERPRODUCESKILL_H

#include "skills/produce/power/PowerProduceSkill.h"

class SolarPowerProduceSkill : public PowerProduceSkill
{
    Q_OBJECT

public:
    SolarPowerProduceSkill( KUnitConversion::UnitId production_unit = KUnitConversion::Watt , Agent *skilled_agent = 0);

    // METHODS
    virtual BaseUnit produce();
};

#endif // SOLARPOWERPRODUCESKILL_H
