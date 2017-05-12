#ifndef ECAR_H
#define ECAR_H

#include "agents/vehicles/Vehicle.h"

class ECar : public Vehicle
{
    Q_OBJECT
public:
    ECar(GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // ECAR_H
