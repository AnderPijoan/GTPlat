#ifndef GTPLATVEHICLE_H
#define GTPLATVEHICLE_H

#include "agents/vehicles/Vehicle.h"

class GTPlatVehicle : public Vehicle
{
    Q_OBJECT
public:
    GTPlatVehicle(QObject* parent = 0);
    ~GTPlatVehicle();

    // METHODS
    virtual void setTravellingThrough(TransportLine *line, double traffic_level);

    double gtplat_total_cc;
    QMap<QDateTime , double > gtplat_cc_log;
    double gtplat_avg_speed;
    QDateTime gtplat_started_datetime;
    double gtplat_spent_time;
};

#endif // GTPLATVEHICLE_H
