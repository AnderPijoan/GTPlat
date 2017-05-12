#ifndef WALKPATH_H
#define WALKPATH_H

#include "agents/transport_networks/walk_paths/WalkPathConstants.h"
#include "agents/transport_networks/TransportLine.h"

class WalkPath : public TransportLine
{
        Q_OBJECT
public:
    WalkPath(WalkPathConstants::walk_path_types type, TransportNode* from, TransportNode* to, GSSLineString* geom, QObject* parent = 0);

    virtual QJsonObject toJSON();
    virtual void fromJSON(QJsonObject json);

    // GETTERS
    WalkPathConstants::walk_path_types getWalkPathType();
    virtual double getCost() const;

    // SETTERS
    void setWalkPathType(WalkPathConstants::walk_path_types walk_path_type);

private:

    WalkPathConstants::walk_path_types walk_path_type; // WalkPath type

};

#endif // WALKPATH_H
