#ifndef DRIVESKILL_H
#define DRIVESKILL_H

#include "skills/Skill.h"

class DriveSkill : public Skill
{
    Q_OBJECT

public:
    DriveSkill(Agent* parent);

    // GETTERS
    double getSpeedFactor();
    LengthUnit getStopDistanceFactor();

    // SETTERS
    void setSpeedFactor(double factor);
    void setStopDistance(LengthUnit distance);

    // METHODS
    double calculateAccelerateForce( SpeedUnit vehicle_speed , SpeedUnit road_max_speed , LengthUnit free_distance );
    double calculateBrakeForce( SpeedUnit vehicle_speed , SpeedUnit road_max_speed , LengthUnit free_distance );

protected:

    // Driver
    // How much it adjusts to road speed (modelled as a probability density function among the roads max speed)
    double speed_factor; // Factor [0,1] how much to adjust to the roads max speed (1+ for overspeeding)
    LengthUnit stop_distance; // Distance on which start stopping before obstacle
};

#endif // DRIVESKILL_H
