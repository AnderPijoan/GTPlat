#ifndef POLLUTIONPRODUCESKILL_H
#define POLLUTIONPRODUCESKILL_H

#include "skills/produce/ProduceSkill.h"
#include "skills/produce/pollution/PollutionConstants.h"

class PollutionProduceSkill : public ProduceSkill
{
    Q_OBJECT

public:
    PollutionProduceSkill(PollutionConstants::pollutant_types pollutant , KUnitConversion::UnitId production_unit = KUnitConversion::Gram , Agent *skilled_agent = 0);

    // GETTERS
    PollutionConstants::pollutant_types getPollutantType();

protected:

    PollutionConstants::pollutant_types pollutant;

};

#endif // POLLUTIONPRODUCESKILL_H
