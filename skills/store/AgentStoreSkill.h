#ifndef AGENTSTORESKILL_H
#define AGENTSTORESKILL_H

#include "QMutex"

#include "skills/Skill.h"
#include "utils/storage/AgentStorage.h"

/**
 * @brief The AgentStoreSkill class
 * This skill makes entities available to contain other agents.
 * It will be used for roads, pipelines, buildings, etc.
 */
class AgentStoreSkill : public Skill
{
    Q_OBJECT

public:
    AgentStoreSkill(Agent* parent);
    ~AgentStoreSkill();

    // GETTERS
    bool isFull() const;
    bool isEmpty() const;
    bool isAgentInside( Agent* agent ) const;
    unsigned int getMaxCapacity() const;
    unsigned int countAgentsInside() const;
    const QList<Agent*> getAgentsInsideByClass( QString class_name ) const;
    const QList<Agent*> getAgentsInside() const;

    // SETTERS
    void setMaxCapacity(unsigned int capacity);

    // SKILL METHODS
    virtual bool enter(Agent* agent);
    virtual bool exit(Agent* agent);
    void clear();

private:
    unsigned int max_capacity;
    AgentStorage* storage;

    // Mutex
    QMutex mutex;
};

#endif // AGENTSTORESKILL_H
