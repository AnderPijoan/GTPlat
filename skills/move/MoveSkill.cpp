#include "MoveSkill.h"

MoveSkill::MoveSkill(Agent* skilled_agent) : Skill( skilled_agent ){
    this->max_speed = SpeedUnit( 1.1 , MeterPerSecond ); // In m/s ~ 4km/h
    this->current_speed = SpeedUnit( 0 , MeterPerSecond );
}

MoveSkill::~MoveSkill(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

SpeedUnit MoveSkill::getMaxSpeed(){
    return this->max_speed;
}

SpeedUnit MoveSkill::getCurrentSpeed(){
    return this->current_speed;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void MoveSkill::setMaxSpeed(SpeedUnit max_speed){
    this->max_speed = max_speed;
}


/**********************************************************************
 METHODS
**********************************************************************/

SpeedUnit MoveSkill::accelerate(double force){
    double normalized_force = qMax( 0.0 , qMin( 1.0 , force ) );

    SpeedUnit s = (this->current_speed + ( ( this->max_speed - this->current_speed ) * normalized_force));
    this->current_speed = SpeedUnit( qMin( this->max_speed , s ) );
    return this->current_speed;
}

SpeedUnit MoveSkill::brake(double force){
    double normalized_force = qMax( 0.0 , qMin( 1.0 , force ) );

    SpeedUnit s = (this->current_speed - (( this->max_speed ) * normalized_force));
    this->current_speed = SpeedUnit( qMax( 0.0 , s.number() ) );
    return this->current_speed;
}

SpeedUnit MoveSkill::brakeToStop(){
    this->current_speed = SpeedUnit( 0 );
    return this->current_speed;
}
