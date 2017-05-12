#ifndef PUBLICTRANSPORTVEHICLEDRIVER_H
#define PUBLICTRANSPORTVEHICLEDRIVER_H

#include "agents/people/vehicle_drivers/ItineraryVehicleDriver.h"
#include "agents/vehicles/public_transport/PublicTransportVehicle.h"
#include "agents/places/constructions/civil_structures/public_transport_stops/PublicTransportStop.h"

QT_FORWARD_DECLARE_CLASS(PublicTransportStop)
QT_FORWARD_DECLARE_CLASS(PublicTransportVehicle)

class PublicTransportVehicleDriver : public ItineraryVehicleDriver
{
    Q_OBJECT

public:
    PublicTransportVehicleDriver(PublicTransportVehicle* vehicle = 0, PersonConstants::gender gender = PersonConstants::MALE, GSSGeometry* geom = 0 ,QObject* parent = 0);
    virtual QJsonObject toJSON();

    // GETTERS
    QString getAgencyName();
    QString getRouteName();
    QString getRouteId();
    QString getRouteVehicleType();

    // SETTERS
    virtual void setVehicle(PublicTransportVehicle* vehicle);
    void setAgencyName(QString agency_name);
    void setRouteName(QString route_name);
    void setRouteId(QString route_id);
    void setRouteVehicleType(QString vehicle_type);

protected slots:
    virtual void stopAtItineraryPoint();
    virtual void departFromItineraryPoint();

protected:

    QString agency_name;
    QString route_name;
    QString route_id;
    QString vehicle_type; // To be asigned a vehicle type

};

#endif // PUBLICTRANSPORTVEHICLEDRIVER_H
