#include "Road.h"

#include "environment/Environment.h"
#include "agents/vehicles/Vehicle.h"

#include "agents/vehicles/Bicycle.h"
#include "agents/vehicles/public_transport/Bus.h"
#include "agents/vehicles/Car.h"
#include "agents/vehicles/ECar.h"
#include "agents/vehicles/Motorcycle.h"
#include "agents/vehicles/Truck.h"

Road::Road(RoadConstants::road_types type, TransportNode* from, TransportNode* to , GSSLineString* geom , QObject* parent) : TransportLine( from , to , geom , parent ){
    this->setRoadType( type );
    this->setMaxSpeed( RoadConstants::defaultSpeed( this->getRoadType() ) );
    this->getStyle()->color = this->getStyle()->border_color = QColor("Navy");
    this->getStyle()->zoom_level = 15;
}

QJsonObject Road::toJSON(){
    QJsonObject agent_json = TransportLine::toJSON();
    agent_json.insert( "road-type",  CustomJsonExporter::format( RoadConstants::toString( this->road_type ) ) );
    return agent_json;
}

void Road::fromJSON(QJsonObject json){
    TransportLine::fromJSON(json);
    if( json.contains( "properties" ) ){
        QJsonObject properties = json["properties"].toObject();
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

RoadConstants::road_types Road::getRoadType() const{
    return this->road_type;
}


/**********************************************************************
 SETTERS
**********************************************************************/

void Road::setRoadType(RoadConstants::road_types road_type){
    this->road_type = road_type;

    switch (this->road_type) {
    case RoadConstants::MOTORWAY:
    case RoadConstants::TRUNK:
        this->setMaxCapacity( 2 );
        break;
    default:
        this->setMaxCapacity( 1 );
        break;
    }

    switch(road_type){

    // MOTORBIKE AND CAR AND TRUCK AND BUS
    case RoadConstants::MOTORWAY:
    case RoadConstants::MOTORWAY_LINK:
    case RoadConstants::MOTORWAY_JUNCTION:
    case RoadConstants::TRUNK:
    case RoadConstants::TRUNK_LINK:
        this->allowed_agent_classes.append( Bus::staticMetaObject.className() );
        this->allowed_agent_classes.append( Car::staticMetaObject.className() );
        this->allowed_agent_classes.append( ECar::staticMetaObject.className() );
        this->allowed_agent_classes.append( Motorcycle::staticMetaObject.className() );
        this->allowed_agent_classes.append( Truck::staticMetaObject.className() );
        break;

        // FOOT AND BIKE AND MOTORBIKE AND CAR AND TRUCK AND BUS
    case RoadConstants::PRIMARY:
    case RoadConstants::PRIMARY_LINK:
    case RoadConstants::SECONDARY:
    case RoadConstants::SECONDARY_LINK:
    case RoadConstants::TERTIARY:
    case RoadConstants::TERTIARY_LINK:
    case RoadConstants::ROUNDABOUT:
    case RoadConstants::RESIDENTIAL:
    case RoadConstants::ROAD:
    case RoadConstants::SERVICE:
        this->allowed_agent_classes.append( Bicycle::staticMetaObject.className() );
        this->allowed_agent_classes.append( Bus::staticMetaObject.className() );
        this->allowed_agent_classes.append( Car::staticMetaObject.className() );
        this->allowed_agent_classes.append( ECar::staticMetaObject.className() );
        this->allowed_agent_classes.append( Motorcycle::staticMetaObject.className() );
        this->allowed_agent_classes.append( Truck::staticMetaObject.className() );
        break;

        // FOOT AND BIKE AND MOTORBIKE AND CAR
    case RoadConstants::LIVING_STREET:
        this->allowed_agent_classes.append( Bicycle::staticMetaObject.className() );
        this->allowed_agent_classes.append( Car::staticMetaObject.className() );
        this->allowed_agent_classes.append( ECar::staticMetaObject.className() );
        this->allowed_agent_classes.append( Motorcycle::staticMetaObject.className() );
        break;

        // FOOT AND BIKE AND MOTORWAY
    case RoadConstants::TRACK:
    case RoadConstants::PATH:
        this->allowed_agent_classes.append( Bicycle::staticMetaObject.className() );
        this->allowed_agent_classes.append( Car::staticMetaObject.className() );
        this->allowed_agent_classes.append( ECar::staticMetaObject.className() );
        this->allowed_agent_classes.append( Motorcycle::staticMetaObject.className() );
        break;

        // BIKE
    case RoadConstants::CYCLEWAY:
    case RoadConstants::CYCLE_LANE:
    case RoadConstants::CYCLE_TRACK:
    case RoadConstants::CYCLE_OPPOSITE_TRACK:
    case RoadConstants::CYCLE_OPPOSITE:
    case RoadConstants::BRIDLEWAY:
        this->allowed_agent_classes.append( Bicycle::staticMetaObject.className() );
        break;

        // ALL REMAINING UNCLASSIFIED ROADS
    case RoadConstants::UNCLASSIFIED:
        break;
    }
}

/**********************************************************************
 METHODS
**********************************************************************/
