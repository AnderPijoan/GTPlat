#ifndef ROAD_H
#define ROAD_H

#include "agents/transport_networks/TransportLine.h"
#include "agents/transport_networks/roads/RoadConstants.h"
#include "skills/store/AgentStoreSkill.h"
#include "agents/vehicles/Vehicle.h"

class Road : public TransportLine
{
    Q_OBJECT
public:
    explicit Road(RoadConstants::road_types type, TransportNode* from, TransportNode* to, GSSLineString* geom = 0, QObject* parent = 0);
    virtual QJsonObject toJSON();
    virtual void fromJSON(QJsonObject json);

    // GETTERS
    RoadConstants::road_types getRoadType() const;

    // SETTERS
    void setRoadType(RoadConstants::road_types road_type);

private:

    RoadConstants::road_types road_type; // Road type

};

#endif // ROAD_H
