#include "GTPlatPublicTransportVehicle.h"
#include "agents/transport_networks/roads/Road.h"
#include "environment/Environment.h"

GTPlatPublicTransportVehicle::GTPlatPublicTransportVehicle(QObject *parent) : Vehicle( parent ){
    this->gtplat_avg_speed = 0;
    this->gtplat_started_datetime = QDateTime::fromString("INVALID");
    this->gtplat_spent_time = 0;
}

GTPlatPublicTransportVehicle::~GTPlatPublicTransportVehicle(){
    //this->gtplat_skill->deleteLater();
}


void GTPlatPublicTransportVehicle::travellingThrough(TransportNetwork* road, double traffic_level){
    /*PublicTransportVehicle::travellingThrough( road , traffic_level );

    Road* casted = dynamic_cast<Road*>(road);

    if( casted ){

        RoadConstants::road_types r = casted->getRoadType();
        double s = this->getCurrentSpeed();

        double g = road->getGradient();
        double f = 0.5;
        double t = traffic_level;
        double l = road->getLength();

        // CONTAMINANTS LOG
        double v = this->gtplat_skill->calculateContaminant( r , s , t , g , f , l );
        v = qMax( 0.0 , v );
        this->gtplat_cc_log.insert( this->getEnvironment()->getTimeEnvironment()->getCurrentDateTime() , v );

        // AVG SPEED
        this->gtplat_avg_speed = (this->gtplat_avg_speed + this->getCurrentSpeed()) / 2;

        // SPENT TIME
        if( !this->gtplat_started_datetime.isValid() ){ this->gtplat_started_datetime = this->getEnvironment()->getTimeEnvironment()->getCurrentDateTime(); }
        this->gtplat_spent_time = this->gtplat_started_datetime.secsTo( this->getEnvironment()->getTimeEnvironment()->getCurrentDateTime() );
    }*/
}
