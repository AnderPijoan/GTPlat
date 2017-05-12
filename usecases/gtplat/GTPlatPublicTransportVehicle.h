#ifndef GTPLATPUBLICTRANSPORTVEHICLE_H
#define GTPLATPUBLICTRANSPORTVEHICLE_H

#include "agents/vehicles/public_transport/PublicTransportVehicle.h"

class GTPlatPublicTransportVehicle : public Vehicle
{
    Q_OBJECT
public:
    GTPlatPublicTransportVehicle(QObject* parent = 0);
    ~GTPlatPublicTransportVehicle();

    // METHODS
    virtual void travellingThrough(TransportNetwork* road , double traffic_level = 1);

    // GTPLAT
    QMap<QDateTime , double > gtplat_cc_log;
    double gtplat_avg_speed;
    QDateTime gtplat_started_datetime;
    double gtplat_spent_time;

};

#endif // GTPLATPUBLICTRANSPORTVEHICLE_H
