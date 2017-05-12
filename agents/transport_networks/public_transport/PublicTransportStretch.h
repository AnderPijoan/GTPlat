#ifndef PUBLICTRANSPORTSTRETCH_H
#define PUBLICTRANSPORTSTRETCH_H

#include "agents/Agent.h"
#include "utils/graph/GSSGraphEdge.h"
#include "utils/geometry/GSSLineString.h"
#include "agents/transport_networks/TransportNode.h"

QT_FORWARD_DECLARE_CLASS(PublicTransportStop)

/**
 * @brief The PublicTransportStretch class Physical connector to create a graph of the PublicTransport lines.
 * Once the route has been decided, the routing can be made using the ordinary roads.
 * PUBLIC TRANSPORT STOPS MUST BE WELL CONNECTED TO ROADS
 */
class PublicTransportStretch : public Agent , public GSSGraphEdge
{
    Q_OBJECT
public:

    PublicTransportStretch( PublicTransportStop* from , PublicTransportStop* to , GSSLineString* geom = 0 , QObject* parent = 0 );

    // STRUCT
    struct StretchService {
        QString route_id;
        QString vehicle_type; // Vehicle type
        QList<int> service_days;
        QDateTime departure;
        QDateTime arrival;
        double price; // In euros, price of using this stretch
    };

    // GETTERS
    virtual double getCost() const;
    PublicTransportStretch::StretchService getNextService( QDateTime time ) const;

    // SETTERS
    void addService( QString route_id , QString vehicle_type , QDateTime departure , QDateTime arrival , double price );

private:
    QList< PublicTransportStretch::StretchService > services;
};

#endif // PUBLICTRANSPORTSTRETCH_H
