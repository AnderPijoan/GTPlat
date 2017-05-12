#ifndef GTPLATSCENARIOCREATOR_H
#define GTPLATSCENARIOCREATOR_H

#include <QObject>
#include <QDate>
#include <QTime>
#include <QThread>

#include "usecases/gtplat/GTPlatAgent.h"
#include "usecases/gtplat/GTPlatIncentive.h"
#include "usecases/gtplat/GTPlatJourney.h"
#include "usecases/gtplat/GTPlatProfile.h"

#include "skills/route/RoutingSkill.h"
#include "skills/route/TransportLineRoutingSkill.h"
#include "utils/websocket_server/WebSocketServer.h"
#include "utils/api/ASyncAPIDriver.h"
#include "utils/fuzzy_logic/FuzzyLogic.h"
#include "utils/random_generators/RouletteWheel.h"

QT_FORWARD_DECLARE_CLASS(Environment)

class GTPlatScenarioCreator : public QObject
{
    Q_OBJECT

public:

    GTPlatScenarioCreator(Environment* environment , WebSocketServer* ws_server);
    QJsonObject toJSON();

    QMap<QString,GTPlatIncentive*> incentives;
    QMap<QString,GTPlatProfile*> profiles;
    QList<GTPlatJourney*> journeys;
    int max_parallel_queries_gtalg;

    // GTALG API DRIVER
    QList<ASyncAPIDriver*> api_drivers;

    // ROUTING GRAPHS
    QMap< QString, GSSDijkstraRoutingGraph*> routing_graphs;

    // ITINERARY STORAGE
    QMap<QString , GTPlatItinerary*> itineraries;

    // PENDING TO BE RUN GTPLATAGENTS
    QHash<GTPlatAgent* , QDateTime> to_be_run;

    // RESULTS
    QMap<QString , int> current_using_leg_mode;
    QMap<QString , int> global_chosen_mode;
    QMap<QString , double> global_impact_cc;

public slots:
    void loadScenario();
    void runScenario( QString class_name , int id  );
    void cycle();

private:
    Environment* environment;
    WebSocketServer* ws_server;

    void createRoutingGraph( QString graph_name , QStringList modes );

};

#endif // GTPLATSCENARIOCREATOR_H
