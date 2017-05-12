#include "PublicTransportStretch.h"

#include "agents/places/constructions/civil_structures/public_transport_stops/PublicTransportStop.h"

PublicTransportStretch::PublicTransportStretch(PublicTransportStop* from , PublicTransportStop* to , GSSLineString* geom , QObject* parent) : Agent( geom , parent ) , GSSGraphEdge( from , to ) {
}



/**********************************************************************
 GETTERS
**********************************************************************/

double PublicTransportStretch::getCost() const{
    double mean_price = 0;
    double mean_time = 0;
    foreach( PublicTransportStretch::StretchService s , this->services ){
        if( mean_price == 0 ){
            mean_price = s.price;
        }
        mean_price = ( mean_price + s.price ) / 2;
        if( mean_time == 0 ){
            mean_time = qAbs( s.arrival.secsTo( s.departure ) );
        }
        mean_time = ( mean_time + qAbs( s.arrival.secsTo( s.departure ) ) ) / 2;
    }
    return ( mean_time * mean_price );
}

PublicTransportStretch::StretchService PublicTransportStretch::getNextService(QDateTime time) const{
    PublicTransportStretch::StretchService match;
    foreach( PublicTransportStretch::StretchService s , this->services ){
        if( s.departure.secsTo( time ) < match.departure.secsTo( time ) ){
            match = s;
        }
    }
    return match;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void PublicTransportStretch::addService(QString route_id , QString vehicle_type , QDateTime departure , QDateTime arrival , double price){
    PublicTransportStretch::StretchService service;
    service.route_id = route_id;
    service.vehicle_type = vehicle_type;
    service.departure = departure;
    service.arrival = arrival;
    service.price = price;
    this->services.append( service );

    this->getStyle()->border_weight = this->services.size() / 100;
}
