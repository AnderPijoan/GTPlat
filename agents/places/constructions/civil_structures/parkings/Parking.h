#ifndef PARKINGSPOT_H
#define PARKINGSPOT_H

#include "agents/places/constructions/civil_structures/CivilStructure.h"
#include "agents/transport_networks/TransportNode.h"
#include "utils/units/UnitConstants.h"

class Parking : public CivilStructure , public TransportNode
{
    Q_OBJECT

public:
    Parking(GSSGeometry* geom = 0 , QObject* parent = 0);

    // GETTERS
    bool isFull();

    // SETTERS

private:

    LengthUnit width;
    LengthUnit length;
};

#endif // PARKINGSPOT_H
