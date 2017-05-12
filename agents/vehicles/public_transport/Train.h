#ifndef TRAIN_H
#define TRAIN_H

#include "agents/vehicles/public_transport/PublicTransportVehicle.h"

class Train : public PublicTransportVehicle
{
    Q_OBJECT
public:
    Train(GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // TRAIN_H
