#ifndef PRODUCESKILL_H
#define PRODUCESKILL_H

#include "skills/Skill.h"
#include "utils/units/UnitConstants.h"

class ProduceSkill : public Skill
{
    Q_OBJECT
public:
    ProduceSkill(KUnitConversion::UnitId production_unit , Agent *skilled_agent = 0);

    // GETTERS
    KUnitConversion::UnitId getUnit() const;
    BaseUnit getProducedTotal() const;

    // METHODS
    virtual BaseUnit produce();

protected:
    const KUnitConversion::UnitId production_unit;
    BaseUnit produced_total; // For accumulating

};

#endif // PRODUCESKILL_H
