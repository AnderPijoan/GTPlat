#ifndef FUNICULAR_H
#define FUNICULAR_H

#include "agents/vehicles/public_transport/PublicTransportVehicle.h"

class Funicular : public PublicTransportVehicle
{
    Q_OBJECT
public:
    Funicular(GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // FUNICULAR_H
