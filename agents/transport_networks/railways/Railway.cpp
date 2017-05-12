#include "Railway.h"

#include "agents/vehicles/public_transport/Funicular.h"
#include "agents/vehicles/public_transport/Metro.h"
#include "agents/vehicles/public_transport/Train.h"
#include "agents/vehicles/public_transport/Tram.h"

Railway::Railway(RailwayConstants::railway_types type, TransportNode* from, TransportNode* to , GSSLineString* geom , QObject* parent) : TransportLine( from , to , geom , parent ){
    this->setRailwayType( type );
    this->setMaxSpeed( RailwayConstants::defaultSpeed( this->getRailwayType() ) );
    this->getStyle()->color = this->getStyle()->border_color = QColor("Brown");
    this->getStyle()->zoom_level = 15;
}

QJsonObject Railway::toJSON(){
    QJsonObject agent_json = TransportLine::toJSON();
    agent_json.insert( "railway-type", CustomJsonExporter::format( RailwayConstants::toString( this->railway_type ) ) );
    return agent_json;
}

void Railway::fromJSON(QJsonObject json){
    TransportLine::fromJSON(json);
    if( json.contains( "properties" ) ){
        QJsonObject properties = json["properties"].toObject();
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

RailwayConstants::railway_types Railway::getRailwayType(){
    return this->railway_type;
}


/**********************************************************************
 SETTERS
**********************************************************************/

void Railway::setRailwayType(RailwayConstants::railway_types railway_type){
    this->railway_type = railway_type;
    switch(railway_type){

    // METRO
    case RailwayConstants::SUBWAY:
        this->allowed_agent_classes.append( Metro::staticMetaObject.className() );
        break;

    // TRAINS
    case RailwayConstants::NARROW_GAUGE:
    case RailwayConstants::LIGHT_RAIL:
    case RailwayConstants::RAIL:
    case RailwayConstants::PRESERVED:
        this->allowed_agent_classes.append( Train::staticMetaObject.className() );
        break;

    // TRAM
    case RailwayConstants::TRAM:
        this->allowed_agent_classes.append( Tram::staticMetaObject.className() );
        break;

    // FUNICULAR
    case RailwayConstants::FUNICULAR:
        this->allowed_agent_classes.append( Funicular::staticMetaObject.className() );
        break;

    case RailwayConstants::UNCLASSIFIED:
        break;
    }
}


/**********************************************************************
 METHODS
**********************************************************************/
