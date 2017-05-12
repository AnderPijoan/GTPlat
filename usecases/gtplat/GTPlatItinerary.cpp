#include "GTPlatItinerary.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "agents/places/constructions/civil_structures/public_transport_stops/BusStop.h"
#include "agents/places/constructions/civil_structures/public_transport_stops/RailwayStop.h"
#include "utils/storage/SharedObjectStorage.h"
#include "utils/routing/GSSDijkstraRoutingGraph.h"
#include "usecases/gtplat/GTPlatScenarioCreator.h"

GTPlatItinerary::GTPlatItinerary() : QObject(){
}

GTPlatItinerary::GTPlatItinerary( QByteArray json_byte_array , GTPlatScenarioCreator* scenario ) : QObject(){

    QJsonDocument doc = QJsonDocument::fromJson( json_byte_array );
    QJsonObject obj = doc.object();

    // REQUEST
    QJsonObject request = obj.value( "requestParameters" ).toObject();
    this->requested_optimization = request.value("weightOptimization").toString();
    this->requested_mode = request.value("mode").toString();

    // RESPONSE
    QJsonObject plan = obj.value( "plan" ).toObject();
    QJsonArray itineraries = plan.value("itineraries").toArray();

    for( int i = 0 ; i < itineraries.size() ; i++ ){
        QJsonObject itinerary = itineraries.at( i ).toObject();



        this->expected_price = CurrencyUnit( itinerary.value("fare").toObject().value("regular").toObject().value("cents").toDouble(0) );
        this->expected_duration = TimeUnit( itinerary.value("duration").toDouble() );
        this->expected_distance = LengthUnit( itinerary.value("totalDistance").toDouble() );
        this->expected_pollution_cc = itinerary.value("totalCarbonEfaCC").toDouble();
        this->expected_pollution_daly = itinerary.value("totalCarbonEfaDaly").toDouble();

        QDateTime last_leg_time = QDateTime::fromMSecsSinceEpoch( itinerary.value("startTime").toDouble() );

        QJsonArray legs = itinerary.value("legs").toArray();
        for(int j = 0 ; j < legs.size() ; j++ ) {

            QJsonObject l = legs.at( j ).toObject();

            // If leg starts later than last time, create a waiting leg
            QDateTime leg_start_time = QDateTime::fromMSecsSinceEpoch( l.value("startTime").toDouble() );
            if( leg_start_time > last_leg_time ){
                Leg wait_leg;
                wait_leg.start = GSSCoordinate( l.value("from").toObject().value("lon").toDouble() , l.value("from").toObject().value("lat").toDouble() );
                wait_leg.end = wait_leg.start;
                wait_leg.distance = LengthUnit( 0 );
                wait_leg.duration = TimeUnit( qAbs( last_leg_time.secsTo( leg_start_time ) ) );
                wait_leg.pollution_cc = 0;
                wait_leg.pollution_daly = 0;
                wait_leg.mode = "WALK";
                this->expected_modes.append( wait_leg.mode );
                this->legs.append( wait_leg );
            }

            // Now parse the real leg
            Leg leg;
            leg.start = GSSCoordinate( l.value("from").toObject().value("lon").toDouble() , l.value("from").toObject().value("lat").toDouble() );
            leg.end = GSSCoordinate( l.value("to").toObject().value("lon").toDouble() , l.value("to").toObject().value("lat").toDouble() );
            leg.distance = LengthUnit( l.value("distance").toDouble() );
            leg.duration = TimeUnit( l.value("duration").toDouble() );
            leg.pollution_cc = l.value("carbonEfaCC").toDouble();
            leg.pollution_daly = l.value("carbonEfaDaly").toDouble();
            leg.mode = l.value("mode").toString("WALK");

            // IF IT IS A PRIVATE VEHICLE, ADD PRICE to total
            if( leg.mode == "CAR" ){ leg.price = leg.price + CurrencyUnit( leg.distance.number() * 0.00029 ); } // 0,29€ PER KM
            if( leg.mode == "MOTORCYCLE" ){ leg.price = leg.price + CurrencyUnit( leg.distance.number() * 0.00020 ); } // 0,20€ PER KM

            //http://www.20minutos.es/noticia/359385/0/tren/metro/bilbao/

            if( leg.mode == "BUS" ){ leg.price = leg.price + CurrencyUnit( leg.distance.number() * ( 0.087 / 1000 ) ); }
            if( leg.mode == "SUBWAY" ){ leg.price = leg.price + CurrencyUnit( leg.distance.number() * ( 0.028 / 1000 ) ); }
            if( leg.mode == "RAIL" ){ leg.price = leg.price + CurrencyUnit( leg.distance.number() * 0.052 / 1000 ); }
            if( leg.mode == "TRAM" ){ leg.price = leg.price + CurrencyUnit( leg.distance.number() * 0.098 / 1000 ); }

            this->expected_modes.append( leg.mode );
            this->legs.append( leg );
            this->route_found = true;
        }

        // ADD PARKING PRICE
        if( this->expected_modes.contains( "CAR" ) ){ this->expected_parking_price = this->expected_parking_price + CurrencyUnit( 4 ); }

        // IF THERE ARE INCENTIVES, APPLY THEM
        if( scenario ){
            for( int x = 0 ; x < this->legs.size() ; x++){
                GTPlatItinerary::Leg leg = this->legs.at( x );
                foreach(GTPlatIncentive* it , scenario->incentives.values() ){
                    // Public Transport
                    if( leg.mode == "BUS" || leg.mode == "RAIL" || leg.mode == "SUBWAY" || leg.mode == "TRAM" ){
                        this->expected_duration = this->expected_duration - leg.duration + (leg.duration * it->public_transport_time_mult);
                        this->expected_duration = this->expected_duration - leg.duration + (leg.duration + it->public_transport_time_add);
                        this->expected_price = this->expected_price - leg.price + (leg.price * it->public_transport_price_mult);
                        this->expected_price = this->expected_price - leg.price + (leg.price + it->public_transport_price_add);
                    }

                    // If walk between two transports ( that is transfer )
                    if( leg.mode == "WALK" && x != 0 && x != this->legs.size()-1 ){
                        this->expected_duration = this->expected_duration - leg.duration + (leg.duration * it->transfer_time_mult);
                        this->expected_duration = this->expected_duration - leg.duration + (leg.duration + it->transfer_time_add);
                    }

                    // Private Vehicles
                    if( leg.mode == "CAR" || leg.mode == "MOTORCYCLE" ){
                        this->expected_duration = this->expected_duration - leg.duration + (leg.duration * it->private_vehicle_time_mult);
                        this->expected_duration = this->expected_duration - leg.duration + (leg.duration + it->private_vehicle_time_add);
                        this->expected_price = this->expected_price - leg.price + (leg.price * it->private_vehicle_price_mult);
                        this->expected_price = this->expected_price - leg.price + (leg.price + it->private_vehicle_price_add);
                    }
                }
            }
        }
    }
}


GTPlatItinerary::GTPlatItinerary(GSSCoordinate from, GSSCoordinate to , GTPlatScenarioCreator* scenario ){
    this->expected_price = CurrencyUnit( from.distance( to ) );
    Leg leg;
    leg.start = from;
    leg.end = to;

    leg.duration =  from.distance( to );
    leg.distance =  from.distance( to ) ;
    leg.pollution_cc =  from.distance( to ) ;
    leg.pollution_daly =  from.distance( to ) ;

    leg.mode = "CAR";
    this->legs.append( leg );
}

void GTPlatItinerary::loadRoutes(GTPlatScenarioCreator *scenario, Environment *environment){

    QList<GTPlatItinerary::Leg> legs_with_route;

    foreach( Leg leg , this->legs ){

        // If TRANSIT, set PublicTransportStops
        if( leg.mode == "SUBWAY" || leg.mode == "RAIL" || leg.mode == "TRAM" ){
            leg.from_stop = dynamic_cast<RailwayStop*>( environment->getNetworkEnvironment()->getNearestNodeFromGraph( leg.start , RailwayStop::staticMetaObject.className() ) );
            leg.to_stop = dynamic_cast<RailwayStop*>( environment->getNetworkEnvironment()->getNearestNodeFromGraph( leg.end , RailwayStop::staticMetaObject.className() ) );
        }
        if( leg.mode == "BUS" ){
            leg.from_stop = dynamic_cast<BusStop*>( environment->getNetworkEnvironment()->getNearestNodeFromGraph( leg.start , BusStop::staticMetaObject.className() ) );
            leg.to_stop = dynamic_cast<BusStop*>( environment->getNetworkEnvironment()->getNearestNodeFromGraph( leg.end , BusStop::staticMetaObject.className() ) );
        }

        // IF VEHICLE set Edges
        if( leg.mode == "WALK" || leg.mode == "BICYCLE" || leg.mode == "ELECTRIC" || leg.mode == "MOTORCYCLE" || leg.mode == "CAR" ){
            GSSDijkstraRoutingGraph* g = scenario->routing_graphs.value( leg.mode );
            if( g ){
                GSSGraphNode* from_node = g->findNearestNode( leg.start );
                GSSGraphNode* to_node = g->findNearestNode( leg.end );
                leg.edges = g->dijkstraShortestPath( from_node , to_node );
            } else {
                qWarning() << Q_FUNC_INFO << "No graph found for leg mode" << leg.mode;
            }

        }

        legs_with_route.append( leg );
    }

    this->legs = legs_with_route;
}

LengthUnit GTPlatItinerary::getEdgesLength(){
    LengthUnit total_length = LengthUnit(0);

    foreach( Leg leg , this->legs ){
        foreach( GSSGraphEdge* e , leg.edges ){
            total_length = total_length + e->getLength();
        }
    }
    return total_length;
}


/**********************************************************************
 GETTERS
**********************************************************************/

TimeUnit GTPlatItinerary::getExpectedDuration(){
    TimeUnit t;
    foreach( GTPlatItinerary::Leg l , this->legs ){
        t = t + l.duration;
    }
    return t;
}

LengthUnit GTPlatItinerary::getExpectedLength(){
    LengthUnit t;
    foreach( GTPlatItinerary::Leg l , this->legs ){
        t = t + l.distance;
    }
    return t;
}

CurrencyUnit GTPlatItinerary::getExpectedPrice(){
    return this->expected_price + this->expected_parking_price;
}

double GTPlatItinerary::getExpectedPollutionCC(){
    double t;
    foreach( GTPlatItinerary::Leg l , this->legs ){
        t = t + l.pollution_cc;
    }
    return t;
}

double GTPlatItinerary::getExpectedPollutionDALY(){
    double t;
    foreach( GTPlatItinerary::Leg l , this->legs ){
        t = t + l.pollution_daly;
    }
    return t;
}
