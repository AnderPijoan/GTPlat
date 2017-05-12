#ifndef GTPLATJOURNEY_H
#define GTPLATJOURNEY_H

#include <QObject>

#include "usecases/gtplat/GTPlatProfile.h"
#include "agents/places/Place.h"

class GTPlatJourney : public QObject
{
    Q_OBJECT
public:
    explicit GTPlatJourney(QObject *parent = 0);

    QJsonObject toJSON();
    void fromJSON(QJsonObject json);

    GTPlatProfile* profile;
    Place* from;
    Place* to;
    QTime time;
    QDate date;
    QString train_with;
    int time_deviation;
    int amount;
    double multiply_by;

};

#endif // GTPLATJOURNEY_H
