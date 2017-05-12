#ifndef TRANSPORTLINE_H
#define TRANSPORTLINE_H

#include "agents/Agent.h"
#include "utils/graph/GSSGraphEdge.h"
#include "utils/geometry/GSSLineString.h"
#include "agents/transport_networks/TransportNode.h"
#include "utils/units/UnitConstants.h"

class TransportLine : public Agent , public GSSGraphEdge
{
    Q_OBJECT

public:

    ~TransportLine();
    virtual QJsonObject toJSON();
    virtual void fromJSON(QJsonObject json);

    // GETTERS
    virtual TransportNode* getFromNode() const;
    virtual TransportNode* getToNode() const;
    const QStringList getAllowedAgents() const;
    bool isAllowed( QString class_name ) const;
    bool isAllowed( Agent* agent ) const;
    virtual double getGradient() const; // Positive for climbing up and negative for going down
    SpeedUnit getMaxSpeed() const;
    CurrencyUnit getCrossingPrice() const;
    bool isTemporarilyClosed() const;
    bool isPermanentlyClosed() const;
    bool isAgentInside( Agent* agent ) const;
    bool isFull() const;
    unsigned int getMaxCapacity() const;
    QList<Agent*> getAgentsInside() const;
    virtual double getCost() const;

    // SETTERS
    void setMaxSpeed(SpeedUnit max_speed);
    void setMaxCapacity(unsigned int capacity);
    void setCrossingPrice(CurrencyUnit crossing_price);
    void setTemporarilyClosed(bool temporarily_closed);
    void setPermanentlyClosed(bool permanently_closed);

    // METHODS
    virtual bool enter( Agent* entering_vehicle);
    virtual bool exit( Agent* exiting_vehicle);

protected:
    TransportLine(TransportNode* from, TransportNode* to, GSSLineString* geom = 0 , QObject* parent = 0);
    QStringList allowed_agent_classes;

private:
    SpeedUnit max_speed; // In meter/second
    CurrencyUnit crossing_price; // In euros, if it has a crossing price
    bool temporarily_closed; // Temporarily closed - Have to wait (Traffic lights, crossings, etc)
    bool permanently_closed; // Permanently closed - Need to recalculate route (works, accidents, etc.)

};

#endif // TRANSPORTLINE_H
