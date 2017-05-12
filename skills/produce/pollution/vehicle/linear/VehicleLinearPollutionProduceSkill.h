#ifndef VEHICLELINEARPOLLUTIONPRODUCESKILL_H
#define VEHICLELINEARPOLLUTIONPRODUCESKILL_H

#include "skills/produce/pollution/vehicle/VehiclePollutionProduceSkill.h"
#include "agents/transport_networks/roads/RoadConstants.h"

class VehicleLinearPollutionProduceSkill : public VehiclePollutionProduceSkill
{
    Q_OBJECT

public:
    VehicleLinearPollutionProduceSkill(PollutionConstants::pollutant_types pollutant , KUnitConversion::UnitId production_unit = KUnitConversion::Gram , Vehicle* vehicle = 0);

    // SETTERS
    virtual void setFactorsForRoadType(RoadConstants::road_types road_type, double x0 , double x1, double x2, double x3, double x4);

    // METHODS
    // (x0 + x1 * vehicle_speed + x2 * vehicle_traffic + x3 * fixed_vehicle_gradient + x4 * vehicle_fill_amount) * length;
    virtual BaseUnit produce(RoadConstants::road_types road_type, SpeedUnit vehicle_speed, double vehicle_traffic, double vehicle_gradient, double vehicle_fill_amount, LengthUnit length);

protected:

    // Function factors
    QMap<RoadConstants::road_types , double> x0;
    QMap<RoadConstants::road_types , double> x1;
    QMap<RoadConstants::road_types , double> x2;
    QMap<RoadConstants::road_types , double> x3;
    QMap<RoadConstants::road_types , double> x4;
};

#endif // VEHICLELINEARPOLLUTIONPRODUCESKILL_H
