#ifndef TRUCK_H
#define TRUCK_H

#include "agents/vehicles/Vehicle.h"

class Truck : public Vehicle
{
    Q_OBJECT
public:
    Truck(GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // TRUCK_H
