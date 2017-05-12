#ifndef POWERPRODUCESKILL_H
#define POWERPRODUCESKILL_H

#include "skills/produce/ProduceSkill.h"
#include "utils/units/UnitConstants.h"

class PowerProduceSkill : public ProduceSkill
{
    Q_OBJECT

public:
    PowerProduceSkill(KUnitConversion::UnitId production_unit = KUnitConversion::Watt , Agent *skilled_agent = 0);

    // GETTERS

    // METHODS
    virtual BaseUnit produce();

protected:

};

#endif // POWERPRODUCESKILL_H
