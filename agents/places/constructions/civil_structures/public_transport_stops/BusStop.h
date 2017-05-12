#ifndef BUSSTOP_H
#define BUSSTOP_H

#include "agents/places/constructions/civil_structures/public_transport_stops/PublicTransportStop.h"

class BusStop : public PublicTransportStop
{
    Q_OBJECT

public:

    BusStop(GSSGeometry* geom = 0 , QObject* parent = 0);

};

#endif // BUSSTOP_H
