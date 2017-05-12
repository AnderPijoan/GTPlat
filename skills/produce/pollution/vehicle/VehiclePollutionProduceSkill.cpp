#include "VehiclePollutionProduceSkill.h"

#include <QtMath>

#include "agents/vehicles/Vehicle.h"

VehiclePollutionProduceSkill::VehiclePollutionProduceSkill(PollutionConstants::pollutant_types pollutant , KUnitConversion::UnitId production_unit , Vehicle* vehicle) : PollutionProduceSkill( pollutant , production_unit , vehicle ){
}

/**********************************************************************
 METHODS
**********************************************************************/

/**
 * Returns the amount polluted for the given road segment and stores this amount in the @
 * @brief VehiclePolluteSkill::pollute
 * @param road_type
 * @param vehicle_speed
 * @param vehicle_traffic
 * @param vehicle_gradient
 * @param vehicle_fill_amount
 * @param length
 * @return
 */
BaseUnit VehiclePollutionProduceSkill::produce(RoadConstants::road_types road_type, SpeedUnit vehicle_speed, double vehicle_traffic, double vehicle_gradient, double vehicle_fill_amount, LengthUnit length){
    Q_UNUSED(road_type)
    Q_UNUSED(vehicle_speed)
    Q_UNUSED(vehicle_traffic)
    Q_UNUSED(vehicle_gradient)
    Q_UNUSED(vehicle_fill_amount)
    Q_UNUSED(length)
    return MassUnit(0);
}
