#ifndef VEHICLE_H
#define VEHICLE_H

#include "agents/Agent.h"

#include "skills/store/AgentStoreSkill.h"
#include "skills/move/MoveSkill.h"

QT_FORWARD_DECLARE_CLASS(TransportLine)

class Vehicle : public Agent
{
    Q_OBJECT
public:
    Vehicle(GSSGeometry* geom = 0 , QObject* parent = 0);
    ~Vehicle();
    virtual QJsonObject toJSON();
    virtual void fromJSON(QJsonObject json);

    // GETTERS
    SpeedUnit getMaxSpeed() const;
    CurrencyUnit getUsageCost() const;
    const QList<Agent*> getAgentsInside() const;

    // METHODS
    virtual void setTravellingThrough(TransportLine* road , double traffic_level = 1);
    bool enter(Agent* entering_agent);
    bool exit(Agent* exiting_agent);
    void clear();

private slots:
    // Agents behaviour
    virtual void behave();

protected:
    double traffic_level = 1;
    TransportLine* current_edge = 0;
    CurrencyUnit usage_cost = 0; // Per meter, used as an average of maintenance + fuel + others...
};

#endif // VEHICLE_H
