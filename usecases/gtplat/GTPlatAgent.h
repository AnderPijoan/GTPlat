#ifndef GTPLATAGENT_H
#define GTPLATAGENT_H

#include "agents/people/Person.h"
#include "agents/people/vehicle_drivers/VehicleDriver.h"
#include "agents/transport_networks/TransportLine.h"
#include "utils/api/ASyncAPIDriver.h"
#include "utils/routing/GSSDijkstraRoutingGraph.h"
#include "utils/fuzzy_logic/FuzzyLogic.h"

#include "skills/route/WalkPathRoutingSkill.h"
#include "skills/route/TransportLineRoutingSkill.h"

#include "usecases/gtplat/GTPlatProfile.h"
#include "usecases/gtplat/GTPlatItinerary.h"

QT_FORWARD_DECLARE_CLASS(GTPlatScenarioCreator)

class GTPlatAgent : public Person
{
    Q_OBJECT

    friend class GTPlatScenarioCreator;

public:
    GTPlatAgent(GTPlatProfile* profile, QDateTime departure = QDateTime::currentDateTime() , PersonConstants::gender gender = PersonConstants::MALE , GSSGeometry* geom = 0 , QObject* parent = 0);
    ~GTPlatAgent();
    virtual QJsonObject toJSON();

    // INITIAL INFO
    GSSCoordinate departure_place;
    GSSCoordinate destination_place;
    GTPlatProfile* profile;

    // DURING EXETUION GENERATED INFO
    QMap<QNetworkReply* , QString> itinerary_mode;
    QList<GTPlatItinerary*> itineraries;
    GTPlatItinerary* chosen_itinerary;
    QList<GTPlatItinerary::Leg> remaining_legs;

    // Variables to be stored
    QDateTime start_time;
    LengthUnit real_length = 0;
    CurrencyUnit real_price = 0;
    MassUnit real_cc_pollution = 0;
    MassUnit real_daly_pollution = 0;
    GTPlatScenarioCreator* scenario_creator;

    // SKILLS
    WalkPathRoutingSkill* walk_route_skill;
    TransportLineRoutingSkill* vehicle_route_skill;

    // DRIVING VEHICLE
    Vehicle* private_vehicle;

    QNetworkReply* requestGTALGItinerary( APIDriver* api , GSSCoordinate from , GSSCoordinate to , QString option );

private slots:
    virtual void behave();

protected slots:
    virtual void setLegRoute();

    virtual void followWalkRoute();
    virtual void walkRouteEnded();

    virtual void followDriveRoute();
    virtual void driveRouteEnded();

    virtual void waitForPublicTransport();

    virtual void departingPublicTransport(PublicTransportVehicleDriver *driver, PublicTransportVehicle *vehicle, QList<PublicTransportStop *> remaining_stops);
    virtual void reachedStop(PublicTransportVehicleDriver *driver, PublicTransportVehicle *vehicle, PublicTransportStop *reached_stop);

    virtual void routeFinished();

    void requestItineraries();
    void waitForItineraries();
    void choseBestItinerary();

    void printLog( QString file_name );

protected:

    int status = 0;
};

#endif // GTPLATAGENT_H
