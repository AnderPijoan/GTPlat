#ifndef RAILWAYSTOP_H
#define RAILWAYSTOP_H

#include "agents/places/constructions/civil_structures/public_transport_stops/PublicTransportStop.h"

class RailwayStop : public PublicTransportStop
{
    Q_OBJECT
public:
    RailwayStop(GSSGeometry* geom = 0 , QObject* parent = 0);

};

#endif // RAILWAYSTOP_H
