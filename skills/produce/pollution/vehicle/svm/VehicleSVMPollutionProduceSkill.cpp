#include "VehicleSVMPollutionProduceSkill.h"

#include <QDir>
#include <QFile>
#include "utils/exporters/debugger/Debugger.h"

#include "agents/vehicles/Vehicle.h"
#include "agents/vehicles/public_transport/Bus.h"
#include "agents/vehicles/Car.h"
#include "agents/vehicles/ECar.h"
#include "agents/vehicles/public_transport/Ferry.h"
#include "agents/vehicles/public_transport/Funicular.h"
#include "agents/vehicles/public_transport/Metro.h"
#include "agents/vehicles/Motorcycle.h"
#include "agents/vehicles/public_transport/Train.h"
#include "agents/vehicles/public_transport/Tram.h"
#include "agents/vehicles/Truck.h"


VehicleSVMPollutionProduceSkill::VehicleSVMPollutionProduceSkill(PollutionConstants::pollutant_types pollutant , KUnitConversion::UnitId production_unit , Vehicle* vehicle) : VehiclePollutionProduceSkill( pollutant , production_unit , vehicle ){

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
    QDir dir( "../skills/produce/pollution/vehicle/svm/data/" + QString(vehicle->metaObject()->className()) + "/" + PollutionConstants::pollutantToString( this->pollutant ) );
    if( dir.exists() ){

        foreach(RoadConstants::road_types road , road_types){

            {
                SVMClassifier* svm = new SVMClassifier();
                bool loaded = svm->load_model( "../skills/produce/pollution/vehicle/svm/data/" + QString(vehicle->metaObject()->className()) + "/" + PollutionConstants::pollutantToString( this->pollutant ) + "/" + RoadConstants::toString( road ) + ".svm");
                if( loaded ){
                    this->svms.insert( road , svm );
                } else {
                    svm->deleteLater();
                    continue;
                }
            }

            {
                QFile file( "../skills/produce/pollution/vehicle/svm/data/" + QString(vehicle->metaObject()->className()) + "/" + PollutionConstants::pollutantToString( this->pollutant ) + "/" + RoadConstants::toString(road) + ".scale");
                QTextStream in(&file);

                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QList<QString> speeds = in.readLine().split(' ');
                    this->speed_m.insert( road , speeds.at(0).toDouble() );
                    this->speed_s.insert( road , speeds.at(1).toDouble() );

                    QList<QString> gradients = in.readLine().split(' ');
                    this->gradient_m.insert( road , gradients.at(0).toDouble() );
                    this->gradient_s.insert( road , gradients.at(1).toDouble() );

                    QList<QString> traffics = in.readLine().split(' ');
                    this->traffic_m.insert( road , traffics.at(0).toDouble() );
                    this->traffic_s.insert( road , traffics.at(1).toDouble() );

                    file.close();
                }
            }

            {
                QFile file( "../skills/produce/pollution/vehicle/svm/data/" + QString(vehicle->metaObject()->className()) + "/" + PollutionConstants::pollutantToString( this->pollutant ) + "/" + RoadConstants::toString(road) + ".yscale");
                QTextStream in(&file);

                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QList<QString> outputs = in.readLine().split(' ');
                    this->output_m.insert( road , outputs.at(0).toDouble() );
                    this->output_s.insert( road , outputs.at(1).toDouble() );

                    file.close();
                }
            }
        }

    }
}

VehicleSVMPollutionProduceSkill::~VehicleSVMPollutionProduceSkill(){
    qDeleteAll( this->svms.values() );
}


/**********************************************************************
 METHODS
**********************************************************************/

BaseUnit VehicleSVMPollutionProduceSkill::produce(RoadConstants::road_types road_type, SpeedUnit vehicle_speed, double vehicle_traffic, double vehicle_gradient, double vehicle_fill_amount, LengthUnit length){
    Q_UNUSED(vehicle_fill_amount)

    if( this->svms.isEmpty() ){
        return MassUnit(-1);
    }

    if( !this->svms.keys().contains(road_type) ){
        road_type = this->svms.keys().at(0);
    }

    SVMClassifier* svm = this->svms.value( road_type , 0 );
    if( !svm ){
        Debugger::warning( this , Q_FUNC_INFO , QString("No SVM for the given road type %1").arg( RoadConstants::toString( road_type ) ) );
        return MassUnit(-1);
    }

    double scaled_speed = ( vehicle_speed.number() - this->speed_m.value(road_type) ) / this->speed_s.value(road_type);
    double scaled_gradient = ( vehicle_gradient - this->gradient_m.value(road_type) ) / this->gradient_s.value(road_type);
    double scaled_traffic = ( vehicle_traffic - this->traffic_m.value(road_type) ) / this->traffic_s.value(road_type);

    svm_node* node = new svm_node[4];
    node[0].index = 1;
    node[0].value = scaled_speed;
    node[1].index = 2;
    node[1].value = scaled_gradient;
    node[2].index = 3;
    node[2].value = scaled_traffic;
    node[3].index = -1; // NECCESSARY TO TELL DATA END AND AVOID PREDICT CRASH

    double scaled_output = svm->predict( node );

    double output = (scaled_output * this->output_s.value(road_type) ) + this->output_m.value(road_type);
    MassUnit result = output * length.number();

    this->produced_total = this->produced_total + result;
    return result;
}
