#ifndef VIEWROUTEAHEADSKILL_H
#define VIEWROUTEAHEADSKILL_H

#include "skills/Skill.h"
#include "utils/graph/GSSGraphEdge.h"

class ViewRouteAheadSkill : public Skill
{
    Q_OBJECT

public:
    ViewRouteAheadSkill(Agent* agent);

    // GETTERS
    double getTrafficLevel();
    LengthUnit getFreeDistance();
    LengthUnit getViewDistance();
    const QList<GSSGraphEdge*> getViewingEdges();

    // SETTERS
    void setViewDistance(LengthUnit view_distance);

    // METHODS
    void updateViewingRoads(QList<GSSGraphEdge*> remaining_route);

private:

    // Route information
    QList<GSSGraphEdge*> viewing_edges; // Without including current road, the next roads the driver is viewing according to the view_distance
    double current_traffic_level; // 1 = freeflow , 0 = interrupted
    LengthUnit current_free_distance; // Free distance untill stop or ocuppied road

    LengthUnit view_distance; // How many meters ahead it can see

};

#endif // VIEWROUTEAHEADSKILL_H
