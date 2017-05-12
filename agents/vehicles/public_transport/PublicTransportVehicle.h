#ifndef PUBLICTRANSPORTVEHICLE_H
#define PUBLICTRANSPORTVEHICLE_H

#include "agents/vehicles/Vehicle.h"
#include "agents/people/vehicle_drivers/PublicTransportVehicleDriver.h"
#include "agents/places/constructions/civil_structures/public_transport_stops/PublicTransportStop.h"

QT_FORWARD_DECLARE_CLASS(PublicTransportVehicleDriver)
QT_FORWARD_DECLARE_CLASS(GTPlatProfile)
QT_FORWARD_DECLARE_CLASS(PublicTransportStop)

class PublicTransportVehicle : public Vehicle
{
    Q_OBJECT

public:
    PublicTransportVehicle(GSSGeometry* geom = 0 , QObject* parent = 0);

signals:
    void reachedStopSignal( PublicTransportVehicleDriver *driver, PublicTransportVehicle *vehicle, PublicTransportStop* reached_stop );

};

#endif // PUBLICTRANSPORTVEHICLE_H
