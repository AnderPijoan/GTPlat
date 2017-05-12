#include "Skill.h"

Skill::Skill(Agent* skilled_agent) : QObject(skilled_agent){
    this->skilled_agent = skilled_agent;
}

