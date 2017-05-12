#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "agents/transport_networks/TransportLine.h"

/**
 * @brief The TrafficLight class
 * It controls one or more RoadNetworkWays
 */
class TrafficLight : public Agent
{
    Q_OBJECT
public:
    TrafficLight( GSSGeometry* geom = 0 , QObject* parent = 0 );
    ~TrafficLight();

    // GETTERS
    int getGroupAmount();

    // SETTERS
    void addRoadGroup(TimeUnit crossable_duration, QList<TransportLine*> roads = QList<TransportLine*>());

    // METHODS
protected:
    virtual void behave();

private:

    struct TrafficLightGroup {
        QList<TransportLine*> roads;
        TimeUnit duration;
    };

    QList<TrafficLightGroup> road_groups;
    int current_crossable_group_index = 0;
};

#endif // TRAFFICLIGHT_H
