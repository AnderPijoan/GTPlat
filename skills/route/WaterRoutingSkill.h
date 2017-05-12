#ifndef WATERROUTINGSKILL_H
#define WATERROUTINGSKILL_H

#include <QObject>
#include "skills/route/RoutingSkill.h"
#include "agents/sewerage_networks/Water.h"
#include "agents/sewerage_networks/WaterConduit.h"

class WaterRoutingSkill : public RoutingSkill
{
    Q_OBJECT
public:
    explicit WaterRoutingSkill( Water* skilled_agent = 0, QString graph_name = WaterConduit::staticMetaObject.className() );

    // METHODS
    void calculateRoute(GSSCoordinate from, GSSCoordinate to);

signals:
    void routeCalculatedSignal();
};

#endif // WATERROUTINGSKILL_H
