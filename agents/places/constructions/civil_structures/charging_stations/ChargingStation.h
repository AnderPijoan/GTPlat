#ifndef CHARGINGSTATION_H
#define CHARGINGSTATION_H

#include "agents/transport_networks/TransportNode.h"
#include "agents/places/constructions/civil_structures/CivilStructure.h"

class ChargingStation : public CivilStructure , public TransportNode
{
    Q_OBJECT

public:
    ChargingStation( GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // CHARGINGSTATION_H
