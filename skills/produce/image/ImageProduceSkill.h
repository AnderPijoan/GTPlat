#ifndef IMAGEPRODUCESKILL_H
#define IMAGEPRODUCESKILL_H

#include "skills/produce/ProduceSkill.h"

class ImageProduceSkill : public ProduceSkill
{
    Q_OBJECT
public:
    explicit ImageProduceSkill(KUnitConversion::UnitId production_unit = KUnitConversion::Hertz , Agent* skilled_agent = 0);

    // METHODS
    virtual FrequencyUnit produce( PowerUnit supplied_power , PowerUnit& consumed_power );

private:
    FrequencyUnit frequency;
};

#endif // IMAGEPRODUCESKILL_H
