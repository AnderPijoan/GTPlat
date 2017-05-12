#ifndef GTPLATINCENTIVE_H
#define GTPLATINCENTIVE_H

#include <QObject>

#include "usecases/gtplat/GTPlatProfile.h"

class GTPlatIncentive : public QObject
{
    Q_OBJECT
public:
    explicit GTPlatIncentive(QObject *parent = 0);

    QJsonObject toJSON();
    void fromJSON(QJsonObject json);

    QString name;
    bool active;

    double public_transport_price_mult = 1;
    double public_transport_price_add = 0;

    double public_transport_time_mult = 1;
    double public_transport_time_add = 0;

    double transfer_time_mult = 1;
    double transfer_time_add = 0;

    double private_vehicle_time_mult = 1;
    double private_vehicle_time_add = 0;

    double private_vehicle_price_mult = 1;
    double private_vehicle_price_add = 0;

};

#endif // GTPLATINCENTIVE_H
