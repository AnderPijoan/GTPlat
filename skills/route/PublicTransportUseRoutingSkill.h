#ifndef PUBLICTRANSPORTUSEROUTINGSKILL_H
#define PUBLICTRANSPORTUSEROUTINGSKILL_H

#include "skills/route/RoutingSkill.h"

#include "agents/transport_networks/walk_paths/WalkPath.h"
#include "agents/transport_networks/public_transport/PublicTransportStretch.h"

class PublicTransportUseRoutingSkill : public RoutingSkill
{
    Q_OBJECT
public:

    PublicTransportUseRoutingSkill( Agent* agent , QString walkpath_graph_class_name = WalkPath::staticMetaObject.className() , QString public_transport_graph_class_name = PublicTransportStretch::staticMetaObject.className() );

    // GETTERS
    QList<PublicTransportStretch*> getRemainingPublicTransportStretchs();

    // METHODS
    virtual void reloadGraph();
    void calculateRoute( GSSCoordinate from, GSSCoordinate to);

protected:
    QList<PublicTransportStretch*> remaining_public_transport_stretchs;

    QString public_transport_graph_class_name;

};

#endif // PUBLICTRANSPORTUSEROUTINGSKILL_H
