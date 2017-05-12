#ifndef TRAM_H
#define TRAM_H

#include "agents/vehicles/public_transport/PublicTransportVehicle.h"

class Tram : public PublicTransportVehicle
{
    Q_OBJECT
public:
    Tram(GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // TRAM_H
