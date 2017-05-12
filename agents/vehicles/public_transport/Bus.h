#ifndef BUS_H
#define BUS_H

#include "agents/vehicles/public_transport/PublicTransportVehicle.h"

class Bus : public PublicTransportVehicle
{
    Q_OBJECT
public:
    Bus(GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // BUS_H
