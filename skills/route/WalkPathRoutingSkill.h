#ifndef WALKPATHROUTINGSKILL_H
#define WALKPATHROUTINGSKILL_H

#include "skills/route/RoutingSkill.h"
#include "agents/people/Person.h"
#include "agents/transport_networks/walk_paths/WalkPath.h"

class WalkPathRoutingSkill : public RoutingSkill
{
    Q_OBJECT

public:
    WalkPathRoutingSkill( Agent* skilled_agent = 0 , QString graph_class_name = WalkPath::staticMetaObject.className() );

    // METHODS
    virtual void reloadGraph();
    virtual WalkPath* getCurrentEdge();
    virtual WalkPath* getFollowingEdge();
};

#endif // WALKPATHROUTINGSKILL_H
