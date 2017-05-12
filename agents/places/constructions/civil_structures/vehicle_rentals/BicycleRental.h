#ifndef BICYCLERENTAL_H
#define BICYCLERENTAL_H

#include "agents/places/constructions/civil_structures/vehicle_rentals/VehicleRental.h"
#include "agents/vehicles/Bicycle.h"

class BicycleRental : public VehicleRental
{
    Q_OBJECT

public:
    BicycleRental(GSSGeometry* geom = 0 , QObject* parent = 0);

    // SETTERS
    virtual Bicycle* rentVehicle();
    virtual bool returnVehicle(Bicycle* bike);

};

#endif // BICYCLERENTAL_H
