#include "DriveSkill.h"

DriveSkill::DriveSkill(Agent *parent) : Skill(parent){
    // Driver
    this->speed_factor = 1; // It will go to the 100% of the speed
    this->stop_distance = LengthUnit( 50 ); // Likes to start stopping 50 meters ahead
}

/**********************************************************************
 GETTERS
**********************************************************************/

double DriveSkill::getSpeedFactor(){
    return this->speed_factor;
}

LengthUnit DriveSkill::getStopDistanceFactor(){
    return this->stop_distance;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void DriveSkill::setSpeedFactor(double factor){
    this->speed_factor = factor;
}

void DriveSkill::setStopDistance(LengthUnit distance){
    this->stop_distance = distance;
}

/**********************************************************************
 METHODS
**********************************************************************/

double DriveSkill::calculateAccelerateForce(SpeedUnit vehicle_speed, SpeedUnit road_max_speed , LengthUnit free_distance){
    if( free_distance <= LengthUnit( 0 )){
        return 0;
    }

    // TODO FIX
    SpeedUnit adjusted_road_max_speed = road_max_speed * this->speed_factor;
    return qMax( 0.0 , (adjusted_road_max_speed - vehicle_speed).number() / adjusted_road_max_speed.number() );
}

double DriveSkill::calculateBrakeForce(SpeedUnit vehicle_speed, SpeedUnit road_max_speed , LengthUnit free_distance){
    if( free_distance <= LengthUnit( 0 ) ){
        return 1;
    }
    if( vehicle_speed <= SpeedUnit( 0 ) ){
        return 0;
    }

    // TODO FIX
    SpeedUnit adjusted_road_max_speed = road_max_speed * this->speed_factor;
    double force_if_freeflow = (vehicle_speed - adjusted_road_max_speed).number() / vehicle_speed.number();
    return qMax( 0.0 , force_if_freeflow );
}
