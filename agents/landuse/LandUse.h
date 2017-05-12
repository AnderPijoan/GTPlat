#ifndef LANDUSE_H
#define LANDUSE_H

#include "agents/Agent.h"
#include "usecases/estolzain/models/LandUseType.h"

class LandUse : public Agent
{
    Q_OBJECT

public:
    LandUse( GSSGeometry* geom, QObject* parent = 0 );
    ~LandUse();
    virtual QJsonObject toJSON();

    // GETTERS
    LandUseType* getLandUseType();

    // SETTERS
    void setLanduseType(LandUseType* landuse_type);

private:
    LandUseType* landuse_type;
};

#endif // LANDUSE_H
