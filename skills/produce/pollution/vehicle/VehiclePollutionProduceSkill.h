#ifndef VEHICLEPOLLUTIONPRODUCESKILL_H
#define VEHICLEPOLLUTIONPRODUCESKILL_H

#include "utils/units/UnitConstants.h"
#include "skills/produce/pollution/PollutionProduceSkill.h"
#include "agents/transport_networks/roads/RoadConstants.h"

QT_FORWARD_DECLARE_CLASS(Vehicle)

class VehiclePollutionProduceSkill : public PollutionProduceSkill
{
    Q_OBJECT

public:

    VehiclePollutionProduceSkill(PollutionConstants::pollutant_types pollutant , KUnitConversion::UnitId production_unit = KUnitConversion::Gram , Vehicle* vehicle = 0);

    // METHODS
    virtual BaseUnit produce(RoadConstants::road_types road_type, SpeedUnit vehicle_speed, double vehicle_traffic, double vehicle_gradient, double vehicle_fill_amount, LengthUnit length);
};

#endif // VEHICLEPOLLUTIONPRODUCESKILL_H
