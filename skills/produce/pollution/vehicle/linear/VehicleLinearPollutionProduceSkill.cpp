#include "VehicleLinearPollutionProduceSkill.h"

#include <QtMath>
#include <QDir>
#include <QStringList>

#include "agents/vehicles/Vehicle.h"
#include "agents/vehicles/public_transport/Bus.h"
#include "agents/vehicles/ECar.h"
#include "agents/vehicles/Car.h"
#include "agents/vehicles/Motorcycle.h"
#include "agents/vehicles/Truck.h"

VehicleLinearPollutionProduceSkill::VehicleLinearPollutionProduceSkill(PollutionConstants::pollutant_types pollutant , KUnitConversion::UnitId production_unit , Vehicle* vehicle) : VehiclePollutionProduceSkill( pollutant , production_unit , vehicle ){

    QList<RoadConstants::road_types> road_types;
    road_types.append( RoadConstants::LIVING_STREET );
    road_types.append( RoadConstants::MOTORWAY );
    road_types.append( RoadConstants::PRIMARY );
    road_types.append( RoadConstants::RESIDENTIAL );
    road_types.append( RoadConstants::SECONDARY );
    road_types.append( RoadConstants::SERVICE );
    road_types.append( RoadConstants::TERTIARY );
    road_types.append( RoadConstants::TRUNK );

    // LOAD SOME DEFAULTS FOR CLIMATE CHANGE
    // WILL RETURN gCO2eq for the given distance in meters
    // BUS : UBus Std >15-18t Euro-II
    // CAR : PC petrol >=2L Euro-2
    // MOTORBIKE : MC 4S <=150cc Euro-2
    // TRUCK : RT >28-32t Euro-II

    // Check if there are values in the data folders for this Skill
    QDir dir( "../skills/pollute/vehicle_pollute/linear/data/" + QString(vehicle->metaObject()->className()) + "/" + PollutionConstants::pollutantToString( this->pollutant ) );
    if( dir.exists() ){

        foreach(RoadConstants::road_types road , road_types){
            QFile file( "../skills/pollute/vehicle_pollute/linear/data/" + QString(vehicle->metaObject()->className()) + "/" + PollutionConstants::pollutantToString( this->pollutant ) + "/" + RoadConstants::toString(road) );
            QTextStream in(&file);

            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QStringList factors = in.readLine().split(' ');
                if( factors.length() < 5 ){
                    qWarning() << "[VehicleLinearPolluteSkill::VehicleLinearPolluteSkill] Data file has missing factors";
                }
                this->setFactorsForRoadType( road , factors[0].toDouble() , factors[1].toDouble() , factors[2].toDouble() , factors[3].toDouble() , factors[4].toDouble() );
                file.close();
            }
        }
    }
}


/**********************************************************************
 SETTERS
**********************************************************************/

/**
 * @brief VehicleContaminantSkill::setFactorsForRoadType IMPORTANT!! THESE FACTORS MUST BE IN METERS AND SECONDS UNITS
 */
void  VehicleLinearPollutionProduceSkill::setFactorsForRoadType(RoadConstants::road_types road_type, double x0, double x1, double x2, double x3, double x4){
    this->x0.insert( road_type , x0 );
    this->x1.insert( road_type , x1 );
    this->x2.insert( road_type , x2 );
    this->x3.insert( road_type , x3 );
    this->x4.insert( road_type , x4 );
}

/**********************************************************************
 METHODS
**********************************************************************/

BaseUnit VehicleLinearPollutionProduceSkill::produce(RoadConstants::road_types road_type, SpeedUnit vehicle_speed, double vehicle_traffic, double vehicle_gradient, double vehicle_fill_amount, LengthUnit length){
    double x0 = this->x0.value( road_type );
    double x1 = this->x1.value( road_type );
    double x2 = this->x2.value( road_type );
    double x3 = this->x3.value( road_type );
    double x4 = this->x4.value( road_type );

    // lineal: s+g+t

    MassUnit result = (x0 +
            x1 * vehicle_speed.number() +
            x2 * vehicle_traffic +
            x3 * vehicle_gradient +
            x4 * vehicle_fill_amount) *
            length.number();

    this->produced_total = this->produced_total + result;
    return result;
}
