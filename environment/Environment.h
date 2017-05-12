#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QCoreApplication>

#include "agents/Agent.h"
#include "environment/execution_environment/ExecutionEnvironment.h"
#include "environment/climate_environment/ClimateEnvironment.h"
#include "environment/time_environment/TimeEnvironment.h"
#include "environment/physical_environment/PhysicalEnvironment.h"
#include "environment/social_environment/SocialEnvironment.h"
#include "environment/network_environment/NetworkEnvironment.h"
#include "utils/storage/AgentStorage.h"

class Environment : public Agent
{
    Q_OBJECT
public:
    Environment( QCoreApplication* parent );
    ~Environment();

    // EXPORTERS
    virtual QJsonObject toJSON();
    virtual QImage toImage( const GSSEnvelope image_bounds , unsigned int image_width = 1024 , unsigned int image_height = 1024 , QStringList class_names = QStringList("Agent") );

    // IMPORTERS
    void fromJSON(QJsonObject json);

    // GETTERS
    const QStringList getClasses() const;
    Agent* getByName(QString agent_name) const; // Get one agent by its name
    Agent* getByClassAndName(QString class_name, QString agent_name) const; // Get one agent by its name
    Agent* getByClassAndId(QString class_name, unsigned int id ) const; // Get one agent
    Agent* getById(unsigned int id) const; // Get one agent
    QList<Agent*> getByClass(QString class_name) const; // Get only one class type agents
    GSSCoordinate getRandomPoint( GSSGeometry* bounds = 0, unsigned int seed = qrand() ) const;

    // SETTERS
    bool addAgent(Agent *agent);
    bool addAgents(QList<Agent*> agents);
    bool setAgentGeometry(Agent* agent , GSSGeometry* new_geom);
    bool runAgent( Agent* agent );
    bool stopAgent( Agent* agent );
    bool removeAgent(Agent* agent);

    // GET SUB ENVIRONMENTS
    const ExecutionEnvironment* getExecutionEnvironment() const;
    const PhysicalEnvironment* getPhysicalEnvironment() const;
    const SocialEnvironment* getSocialEnvironment() const;
    const TimeEnvironment* getTimeEnvironment() const;
    const ClimateEnvironment* getClimateEnvironment() const;
    const NetworkEnvironment* getNetworkEnvironment() const;

signals:
    void pushToInterfaceSignal( QJsonObject data , QString operation );

public slots:
    void runEnvironment();
    void stopEnvironment();

private slots:
    virtual void behave(); // Used to be called periodically

private:

    // Execution Environment
    ExecutionEnvironment* execution_environment;

    // Physical Environment
    PhysicalEnvironment* physical_environmant;

    // Social Environment
    SocialEnvironment* social_environment;

    // Environment time
    TimeEnvironment* time_environment;

    // Environment climate
    ClimateEnvironment* climate_environment;

    // Network Environment
    NetworkEnvironment* network_environment;

    // ENVIRONMENT MEMORY
    AgentStorage* environment_agents;

    // Mutex, for avoiding concurrency
    QMutex mutex;

};

#endif // ENVIRONMENT_H
