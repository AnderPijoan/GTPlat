#ifndef EXECUTIONENVIRONMENT_H
#define EXECUTIONENVIRONMENT_H

#include <QObject>
#include <QMutex>
#include <QDateTime>
#include "agents/Agent.h"
#include "environment/time_environment/TimeEnvironment.h"

QT_FORWARD_DECLARE_CLASS(Environment)

class ExecutionEnvironment : public QObject
{
    Q_OBJECT
    friend class Environment; // Environment can access the constructor

public:
    int getRunningAgents();

    // GETTERS
    double getCycleFrequency() const;

    // SETTERS
    void setCycleFrequency( double cycle_frequency_milliseconds );

private:
    explicit ExecutionEnvironment(Environment *parent);
    ~ExecutionEnvironment();

    // AGENTS
    void addAgent(Agent* agent);
    void removeAgent(Agent* agent);
    void executeAgents(TimeEnvironment* time_environment);

    // RUNNING AGENTS
    QHash<int , Agent*> running_agents;

    // Cycle frequency
    // Frequency to execute Environments behave() method
    double cycle_frequency_milliseconds;

    // Avoid more than one execution at a time
    QMutex mutex;
};

#endif // EXECUTIONENVIRONMENT_H
