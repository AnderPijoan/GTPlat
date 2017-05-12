#ifndef CAR_H
#define CAR_H

#include "agents/vehicles/Vehicle.h"

class Car : public Vehicle
{
    Q_OBJECT

public:
    Car(GSSGeometry* geom = 0, QObject* parent = 0);
};

#endif // CAR_H
