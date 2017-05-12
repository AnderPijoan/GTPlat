#ifndef ROUTINGSKILL_H
#define ROUTINGSKILL_H

#include "skills/Skill.h"

#include "utils/geometry/GSSCoordinate.h"
#include "utils/routing/GSSDijkstraRoutingGraph.h"
#include "utils/storage/SharedObjectStorage.h"

class RoutingSkill : public Skill
{
    Q_OBJECT
public:
    ~RoutingSkill();

    // GETTERS
    QList<GSSGraphEdge*> getRemainingRoute() const;
    LengthUnit getRemainingLength() const; // In meters

    // METHODS
    virtual void setRoute(QList<GSSGraphEdge*> route_edges);
    virtual void reloadGraph();
    virtual void calculateRoute( GSSCoordinate from, GSSCoordinate to);
    virtual GSSGraphEdge* getCurrentEdge(); // Returns current edge (index) untill route end
    virtual GSSGraphEdge* getFollowingEdge(); // Returns the following edge (index + 1)
    virtual void pointToFollowingEdge();

protected:
    RoutingSkill( Agent* skilled_agent = 0 , QString graph_class_name = QObject::staticMetaObject.className() );

    // Graph
    GSSDijkstraRoutingGraph* routing_graph;
    int current_edge_index = -1;
    // Route
    QList<GSSGraphEdge*> route_edges;
    // Graph to be retreieved from NetworkEnvironment
    QString graph_class_name;

};

#endif // ROUTINGSKILL_H
