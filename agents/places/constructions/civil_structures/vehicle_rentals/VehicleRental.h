#ifndef VEHICLERENTAL_H
#define VEHICLERENTAL_H

#include "agents/places/constructions/civil_structures/CivilStructure.h"
#include "agents/vehicles/Vehicle.h"

class VehicleRental : public CivilStructure
{
    Q_OBJECT

public:
    VehicleRental(GSSGeometry* geom = 0 , QObject* parent = 0);

    // GETTERS
    bool isFull();
    bool isEmpty();
    int countVehicles();

    // SETTERS
    virtual Vehicle* rentVehicle();
    virtual bool returnVehicle(Vehicle* vehicle);
};

#endif // VEHICLERENTAL_H
