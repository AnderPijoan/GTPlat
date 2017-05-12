#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H

#include "agents/vehicles/Vehicle.h"

class Motorcycle : public Vehicle
{
    Q_OBJECT
public:
    Motorcycle(GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // MOTORCYCLE_H
