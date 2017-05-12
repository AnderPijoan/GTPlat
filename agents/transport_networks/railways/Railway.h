#ifndef RAILWAY_H
#define RAILWAY_H

#include "agents/transport_networks/railways/RailwayConstants.h"
#include "agents/transport_networks/TransportLine.h"

class Railway : public TransportLine
{
    Q_OBJECT
public:
    Railway(RailwayConstants::railway_types type, TransportNode* from, TransportNode* to, GSSLineString* geom = 0, QObject* parent = 0);
    virtual QJsonObject toJSON();
    virtual void fromJSON(QJsonObject json);

    // GETTERS
    RailwayConstants::railway_types getRailwayType();

    // SETTERS
    void setRailwayType(RailwayConstants::railway_types road_type);

    // METHODS

private:

    RailwayConstants::railway_types railway_type; // Railway type
};

#endif // RAILWAY_H
