#ifndef PUBLICTRANSPORTUSERPERSON_H
#define PUBLICTRANSPORTUSERPERSON_H

#include "agents/people/Person.h"
#include "agents/places/constructions/civil_structures/public_transport_stops/PublicTransportStop.h"
#include "agents/vehicles/public_transport/PublicTransportVehicle.h"
#include "agents/people/vehicle_drivers/PublicTransportVehicleDriver.h"

QT_FORWARD_DECLARE_CLASS(PublicTransportStop)
QT_FORWARD_DECLARE_CLASS(PublicTransportStretch)
QT_FORWARD_DECLARE_CLASS(PublicTransportVehicle)
QT_FORWARD_DECLARE_CLASS(PublicTransportVehicleDriver)

typedef QList<PublicTransportStop*> PublicTransportStopList;

class PublicTransportUserPerson : public Person
{
    Q_OBJECT
public:

    PublicTransportUserPerson(PersonConstants::gender gender = PersonConstants::MALE , GSSGeometry* geom = 0, QObject* parent = 0);
    virtual QJsonObject toJSON();

    // GETTERS
    QDateTime getDeparture();

    // SETTERS
    void setDeparture(QDateTime departure);
    void setDestination(GSSCoordinate destination);

private slots:
    // Agents behaviour
    //virtual void tick();

protected slots:
    virtual void agentStarted();
    virtual void planTrip();
    virtual void calculateRouteToPublicTransportStop();
    virtual void followWalkRoute();
    virtual void retryFollowWalkRoute();
    virtual void waitForPublicTransport();
    virtual void departingPublicTransport( PublicTransportVehicleDriver* driver , PublicTransportVehicle* vehicle , QList<PublicTransportStop*> passing_by_stops );
    virtual void reachedStop( PublicTransportVehicleDriver *driver, PublicTransportVehicle *vehicle, PublicTransportStop* stop );
    virtual void routeFinished();

protected:

    GSSCoordinate destination;

private:

    QList<PublicTransportStretch*> remaining_stretchs;

    // Departure time
    QDateTime departure;

};

#endif // PUBLICTRANSPORTUSERPERSON_H
