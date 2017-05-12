#ifndef SOUNDPRODUCESKILL_H
#define SOUNDPRODUCESKILL_H

#include "skills/produce/ProduceSkill.h"

class SoundProduceSkill : public ProduceSkill
{
    Q_OBJECT
public:
    SoundProduceSkill(KUnitConversion::UnitId production_unit = KUnitConversion::Hertz, Agent* skilled_agent = 0);

    // METHODS
    virtual FrequencyUnit produce( PowerUnit supplied_power , PowerUnit& consumed_power );

private:
    FrequencyUnit max_decibels; // Maximum decibels provided by the device
};

#endif // SOUNDPRODUCESKILL_H
