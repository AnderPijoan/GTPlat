#include "GTPlatVehicle.h"
#include "agents/transport_networks/roads/Road.h"
#include "utils/exporters/logger/Logger.h"
#include "environment/Environment.h"

GTPlatVehicle::GTPlatVehicle(QObject* parent) : Vehicle( parent ){
    this->gtplat_avg_speed = 0;
    this->gtplat_total_cc = 0;
    this->gtplat_started_datetime = QDateTime::fromString("INVALID");
    this->gtplat_spent_time = 0;
}


GTPlatVehicle::~GTPlatVehicle(){
    //this->gtplat_skill->deleteLater();
}


void GTPlatVehicle::travellingThrough(TransportLine *line, double traffic_level){
    /*Vehicle::travellingThrough( road , traffic_level );

    Road* casted = dynamic_cast<Road*>(road);

    if( casted ){

        RoadConstants::road_types r = casted->getRoadType();
        double s = this->getCurrentSpeed();

        double g = road->getGradient();
        double f = 0.5;
        double t = traffic_level;
        double l = road->getLength();

        // CONTAMINANTS LOG
        if( this->gtplat_skill ){
            double v = this->gtplat_skill->calculateContaminant( r , s , t , g , f , l );
            v = qMax( 0.0 , v );
            this->gtplat_total_cc += v;
            this->gtplat_cc_log.insert( this->getEnvironment()->getTimeEnvironment()->getCurrentDateTime() , v );
        }

        // AVG SPEED
        this->gtplat_avg_speed = (this->gtplat_avg_speed + this->getCurrentSpeed()) / 2;

        // SPENT TIME
        if( !this->gtplat_started_datetime.isValid() ){ this->gtplat_started_datetime = this->getEnvironment()->getTimeEnvironment()->getCurrentDateTime(); }
        this->gtplat_spent_time = this->gtplat_started_datetime.secsTo( this->getEnvironment()->getTimeEnvironment()->getCurrentDateTime() );

        // Log vehicle travelling
        // VEHICLE ID , TIMESTAMP , MESSAGE TYPE , MESSAGE
        //QString str = QString("speed=%1;gradient=%2;traffic=%3;length=%4;pollution=%5;avg_speed=%6;total_polluted=%7").arg( s * 3.6 ).arg( g ).arg( t ).arg( l ).arg( v ).arg( this->gtplat_avg_speed * 3.6 ).arg( this->gtplat_total_cc );
        //Logger::log( QString("%1 , %2 , %3 , %4").arg( this->getId() ).arg( TimeController::getCurrentDateTime().toString() ).arg( "TRAVELLING" ).arg( str ) , this->metaObject()->className());

    }*/
}
