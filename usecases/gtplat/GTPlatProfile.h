#ifndef GTPLATPROFILE_H
#define GTPLATPROFILE_H

#include <QObject>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QTime>
#include <QDate>

class GTPlatProfile : public QObject
{
    Q_OBJECT
public:
    explicit GTPlatProfile(QObject *parent = 0);
    GTPlatProfile( QJsonObject json );

    QJsonObject toJSON();

    QStringList options; // [ "WALK" , "WALK,TRANSIT" , "CAR_PARK,WALK,TRANSIT" , ... ]
    double longer;
    double slower;
    double more_expensive;
    double more_polutant;

};

#endif // GTPLATPROFILE_H
