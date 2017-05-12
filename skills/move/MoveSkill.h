#ifndef MOVESKILL_H
#define MOVESKILL_H

#include "skills/Skill.h"
#include "utils/units/UnitConstants.h"

class MoveSkill : public Skill
{
    Q_OBJECT

public:
    MoveSkill(Agent* skilled_agent);
    ~MoveSkill();

    // GETTERS
    SpeedUnit getMaxSpeed();
    SpeedUnit getCurrentSpeed();

    // SETTERS
    SpeedUnit accelerate( double force = 0.5 );
    SpeedUnit brake( double force = 0.5 );
    SpeedUnit brakeToStop();
    void setMaxSpeed( SpeedUnit max_speed ); // In m/s

private:

    // Moving speed
    SpeedUnit max_speed; // In m/s
    SpeedUnit current_speed; // In m/s

};

#endif // MOVESKILL_H
