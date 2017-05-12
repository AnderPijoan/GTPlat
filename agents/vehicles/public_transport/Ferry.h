#ifndef FERRY_H
#define FERRY_H

#include "agents/vehicles/public_transport/PublicTransportVehicle.h"

class Ferry : public PublicTransportVehicle
{
    Q_OBJECT
public:
    Ferry(GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // FERRY_H
