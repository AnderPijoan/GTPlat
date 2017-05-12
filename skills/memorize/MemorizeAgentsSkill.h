#ifndef MEMORIZEAGENTSSKILL_H
#define MEMORIZEAGENTSSKILL_H

#include "skills/memorize/MemorizeSkill.h"
#include "utils/storage/AgentStorage.h"

class MemorizeAgentsSkill : public MemorizeSkill
{
    Q_OBJECT

public:
    MemorizeAgentsSkill(Agent* skilled_agent = 0);

    // GETTERS
    Agent* getByClassAndId(QString class_name , unsigned int id);
    Agent* getById(unsigned int id);
    QList<Agent*> getAgentsByClass(QString class_name);

    // SETTERS
    virtual void add(Agent* agent);
    virtual void remove(Agent* agent);

};

#endif // MEMORIZEAGENTSSKILL_H
