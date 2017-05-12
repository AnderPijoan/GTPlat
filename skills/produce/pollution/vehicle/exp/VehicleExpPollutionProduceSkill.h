#ifndef VEHICLEEXPPOLLUTIONPRODUCESKILL_H
#define VEHICLEEXPPOLLUTIONPRODUCESKILL_H

#include "skills/produce/pollution/vehicle/VehiclePollutionProduceSkill.h"

class VehicleExpPollutionProduceSkill : public VehiclePollutionProduceSkill
{
    Q_OBJECT

public:
    VehicleExpPollutionProduceSkill(PollutionConstants::pollutant_types pollutant , KUnitConversion::UnitId production_unit = KUnitConversion::Gram ,  Vehicle* vehicle = 0);

    // SETTERS
    virtual void setFactorsForRoadType(RoadConstants::road_types road_type, double x0 , double x1, double x2, double x3, double x4, double x5);

    // METHODS
    virtual BaseUnit produce(RoadConstants::road_types road_type, SpeedUnit vehicle_speed, double vehicle_traffic, double vehicle_gradient, double vehicle_fill_amount, LengthUnit length);

protected:
    // Function factors
    QMap<RoadConstants::road_types , double> x0;
    QMap<RoadConstants::road_types , double> x1;
    QMap<RoadConstants::road_types , double> x2;
    QMap<RoadConstants::road_types , double> x3;
    QMap<RoadConstants::road_types , double> x4;
    QMap<RoadConstants::road_types , double> x5;
};

#endif // VEHICLEEXPPOLLUTIONPRODUCESKILL_H
