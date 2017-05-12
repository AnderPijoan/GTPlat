#ifndef POWERSTORESKILL_H
#define POWERSTORESKILL_H

#include "skills/Skill.h"

class PowerStoreSkill : public Skill
{
    Q_OBJECT

public:
    PowerStoreSkill(Agent* skilled_agent = 0);

    // GETTERS
    bool isFullyLoaded();
    double getMaxCapacity();
    double getAmountInside();

    // SETTERS
    void setMaxCapacity(double capacity);

    // SKILL METHODS

private:
    double voltage; // In volts
    double max_capacity; // In watts
    double amount_inside; // In watts
};

#endif // POWERSTORESKILL_H
