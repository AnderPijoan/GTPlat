#ifndef VEHICLEROUTINGSKILL_H
#define VEHICLEROUTINGSKILL_H

#include "skills/route/RoutingSkill.h"
#include "agents/transport_networks/TransportLine.h"

QT_FORWARD_DECLARE_CLASS(Vehicle)

class TransportLineRoutingSkill : public RoutingSkill
{
    Q_OBJECT
public:

    TransportLineRoutingSkill(Vehicle* vehicle , Agent* skilled_agent = 0 , QString graph_class_name = TransportLine::staticMetaObject.className() );

    // GETTERS
    TimeUnit getRemainingTime(); // In seconds
    Value getRemainingCrossingPrice();

    // METHODS
    virtual void reloadGraph();
    virtual TransportLine* getCurrentEdge();
    virtual TransportLine* getFollowingEdge();

private:
    Vehicle* vehicle;
    QList<TransportLine*> avoid_edges;

};

#endif // VEHICLEROUTINGSKILL_H
