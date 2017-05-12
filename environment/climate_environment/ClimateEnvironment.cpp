#include "ClimateEnvironment.h"

#include "environment/Environment.h"
#include "utils/geometry/GSSEnvelope.h"

ClimateEnvironment::ClimateEnvironment(Environment *parent) : QObject(parent){
}

ClimateEnvironment::~ClimateEnvironment(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

TemperatureUnit ClimateEnvironment::getTemperature( GSSCoordinate coor ) const{
    return TemperatureUnit( this->temperature.getValue( coor ) );
}

double ClimateEnvironment::getHumidity( GSSCoordinate coor ) const{
    return this->humidity.getValue( coor );
}

double ClimateEnvironment::getSunIrradiation( GSSCoordinate coor ) const{
    return this->sun_irradiation.getValue( coor );
}


/**********************************************************************
 SETTERS
**********************************************************************/

void ClimateEnvironment::addTemperature(GSSCoordinate coor, TemperatureUnit value){
    this->temperature.setValue( coor , value.number() );
}

void ClimateEnvironment::addHumidity(GSSCoordinate coor, double value){
    this->humidity.setValue( coor , value );
}

void ClimateEnvironment::addSunIrradiation(GSSCoordinate coor, double value){
    this->sun_irradiation.setValue( coor , value );
}
