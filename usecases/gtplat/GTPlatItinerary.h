#ifndef GTPLATITINERARY_H
#define GTPLATITINERARY_H

#include <QObject>
#include <QStringList>
#include "geos/geom/Coordinate.h"
#include <KUnitConversion/Value>

#include "environment/Environment.h"
#include "agents/places/constructions/civil_structures/public_transport_stops/PublicTransportStop.h"

#include "usecases/gtplat/GTPlatProfile.h"

using namespace KUnitConversion;

QT_FORWARD_DECLARE_CLASS(GTPlatScenarioCreator)

class GTPlatItinerary : public QObject
{
    Q_OBJECT
public:
    GTPlatItinerary();
    GTPlatItinerary( QByteArray json_byte_array , GTPlatScenarioCreator* scenario );
    GTPlatItinerary( GSSCoordinate from , GSSCoordinate to , GTPlatScenarioCreator* scenario );
    void loadRoutes( GTPlatScenarioCreator* scenario , Environment* environment );
    LengthUnit getEdgesLength();

    struct Leg {
        QString mode;
        GSSCoordinate start;
        GSSCoordinate end;
        LengthUnit distance;
        TimeUnit duration;
        CurrencyUnit price;
        double pollution_cc;
        double pollution_daly;
        PublicTransportStop* from_stop; // If travelling by Public Transport
        PublicTransportStop* to_stop; // If travelling by Public Transport
        QList<GSSGraphEdge*> edges; // If Travelling by vehicle
    };

    // INIT INFO
    GSSCoordinate start;
    GSSCoordinate end;
    QString requested_mode; // ORIGINAL REQUESTED MODE
    QString requested_optimization; // ORIGINAL REQUESTED OPTIMIZATION

    // AFTER LOADED INFO
    TimeUnit expected_duration = 0;
    LengthUnit expected_distance = 0;
    double expected_pollution_cc;
    double expected_pollution_daly;
    CurrencyUnit expected_price = 0;
    CurrencyUnit expected_parking_price = 0;
    QStringList expected_modes;
    QList<GTPlatItinerary::Leg> legs;

    bool route_found = false;

    // GETTERS

    TimeUnit getExpectedDuration();
    LengthUnit getExpectedLength();
    CurrencyUnit getExpectedPrice();
    double getExpectedPollutionCC();
    double getExpectedPollutionDALY();

};

#endif // GTPLATITINERARY_H
