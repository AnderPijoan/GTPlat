#ifndef BICYCLE_H
#define BICYCLE_H

#include "agents/vehicles/Vehicle.h"

class Bicycle : public Vehicle
{
    Q_OBJECT
public:
    Bicycle(GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // BICYCLE_H
