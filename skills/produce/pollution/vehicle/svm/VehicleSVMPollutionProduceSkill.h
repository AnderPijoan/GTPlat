#ifndef VEHICLESVMPOLLUTIONPRODUCESKILL_H
#define VEHICLESVMPOLLUTIONPRODUCESKILL_H

#include "skills/produce/pollution/vehicle/VehiclePollutionProduceSkill.h"

#include "include/svm/SVMClassifier.h"

class VehicleSVMPollutionProduceSkill : public VehiclePollutionProduceSkill
{
    Q_OBJECT

public:
    VehicleSVMPollutionProduceSkill(PollutionConstants::pollutant_types pollutant , KUnitConversion::UnitId production_unit = KUnitConversion::Gram , Vehicle* vehicle = 0);
    ~VehicleSVMPollutionProduceSkill();

    // METHODS
    virtual BaseUnit produce(RoadConstants::road_types road_type, SpeedUnit vehicle_speed, double vehicle_traffic, double vehicle_gradient, double vehicle_fill_amount, LengthUnit length);

private:
    QMap<RoadConstants::road_types , SVMClassifier*> svms;

    QMap<RoadConstants::road_types , double> speed_m;
    QMap<RoadConstants::road_types , double> speed_s;
    QMap<RoadConstants::road_types , double> gradient_m;
    QMap<RoadConstants::road_types , double> gradient_s;
    QMap<RoadConstants::road_types , double> traffic_m;
    QMap<RoadConstants::road_types , double> traffic_s;
    QMap<RoadConstants::road_types , double> output_m;
    QMap<RoadConstants::road_types , double> output_s;
};

#endif // VEHICLESVMPOLLUTIONPRODUCESKILL_H
