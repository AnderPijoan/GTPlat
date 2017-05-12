#ifndef VEHICLESUMOPOLLUTIONPRODUCESKILL_H
#define VEHICLESUMOPOLLUTIONPRODUCESKILL_H

#include "skills/produce/pollution/vehicle/VehiclePollutionProduceSkill.h"

class VehicleSumoPollutionProduceSkill : public VehiclePollutionProduceSkill
{
    Q_OBJECT

public:
    VehicleSumoPollutionProduceSkill(PollutionConstants::pollutant_types pollutant , KUnitConversion::UnitId production_unit = KUnitConversion::Gram , Vehicle* vehicle = 0);

    // SETTERS
    virtual void setFactorsForRoadType(RoadConstants::road_types road_type, double x0 , double x1, double x2, double x3, double x4, double x5, double x6, double x7);

    // METHODS
    // (x0 + x1 * vehicle_speed + x2 * vehicle_speed * vehicle_speed + x3 * vehicle_speed * vehicle_speed * vehicle_speed + x4 * vehicle_speed * fixed_vehicle_gradient + x5 * vehicle_speed * fixed_vehicle_gradient * fixed_vehicle_gradient + x6 * vehicle_traffic + x7 * vehicle_weight) * length;
    virtual BaseUnit produce(RoadConstants::road_types road_type, SpeedUnit vehicle_speed, double vehicle_traffic, double vehicle_gradient, double vehicle_fill_amount, LengthUnit length);

protected:
    // Function factors
    // Function factors
    QMap<RoadConstants::road_types , double> x0;
    QMap<RoadConstants::road_types , double> x1;
    QMap<RoadConstants::road_types , double> x2;
    QMap<RoadConstants::road_types , double> x3;
    QMap<RoadConstants::road_types , double> x4;
    QMap<RoadConstants::road_types , double> x5;
    QMap<RoadConstants::road_types , double> x6;
    QMap<RoadConstants::road_types , double> x7;

};

#endif // VEHICLESUMOPOLLUTIONPRODUCESKILL_H
