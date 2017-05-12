#include "Place.h"

Place::Place( GSSGeometry* geom , QObject* parent ) : Agent( geom , parent ){

    if( geom ){
        this->area = AreaUnit( geom->getArea() );
    }

    this->getStyle()->color = QColor("Green");
    this->getStyle()->color.setAlphaF( 0.5 );
    this->getStyle()->icon_url = "http://cdn.flaticon.com/png/128/33956.png";
}

QJsonObject Place::toJSON(){
    QJsonObject agent_json = Agent::toJSON();
    agent_json.insert( "area", CustomJsonExporter::format( this->area ) );
    return agent_json;
}

/**********************************************************************
 GETTERS
**********************************************************************/

AreaUnit Place::getArea(){
    return this->area;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Place::setArea(AreaUnit area){
    this->area = area;
}

