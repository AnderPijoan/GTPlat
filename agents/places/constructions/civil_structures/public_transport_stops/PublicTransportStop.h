#ifndef PUBLICTRANSPORTSTOP_H
#define PUBLICTRANSPORTSTOP_H

#include "agents/places/constructions/civil_structures/CivilStructure.h"
#include "agents/transport_networks/TransportNode.h"

QT_FORWARD_DECLARE_CLASS(PublicTransportVehicleDriver)
QT_FORWARD_DECLARE_CLASS(PublicTransportVehicle)

class PublicTransportStop : public CivilStructure , public TransportNode
{
    Q_OBJECT

public:
    PublicTransportStop(GSSGeometry* geom = 0 , QObject* parent = 0);

    // GETTERS
    QString getZoneId();

    // SETTERS
    void setZoneId( QString zone_id );
    bool addWaitingPerson( Agent* agent );
    bool removeWaitingPerson( Agent* agent );

signals:
    void departingPublicTransportSignal( PublicTransportVehicleDriver* driver , PublicTransportVehicle* vehicle , QList<PublicTransportStop*> passing_by_stops );

private:
    QString zone_id; // Stop zone id

};

#endif // PUBLICTRANSPORTSTOP_H
