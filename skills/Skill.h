#ifndef SKILL_H
#define SKILL_H

#include <QObject>
#include "agents/Agent.h"

class Skill : public QObject
{
    Q_OBJECT
public:
    explicit Skill(Agent *skilled_agent = 0);

protected:
    /**
     * @brief skilled_agent Agent that owns this skill
     */
    Agent* skilled_agent;

};

#endif // SKILL_H
