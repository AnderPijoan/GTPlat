#ifndef METRO_H
#define METRO_H

#include "agents/vehicles/public_transport/PublicTransportVehicle.h"

class Metro : public PublicTransportVehicle
{
    Q_OBJECT
public:
    Metro(GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // METRO_H
