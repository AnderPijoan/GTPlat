#include "LandUse.h"

LandUse::LandUse( GSSGeometry* geom, QObject* parent ) : Agent( geom, parent ){
    this->landuse_type = 0;

    // Set UI properties
    this->getStyle()->border_weight = 3;
    this->getStyle()->dash_array = "5 5";
    this->getStyle()->color = this->getStyle()->border_color = UiStyle::randomColor();
    this->getStyle()->color.setAlpha( 180 );
}

LandUse::~LandUse(){
    this->landuse_type->deleteLater();
}

QJsonObject LandUse::toJSON(){
    QJsonObject json = Agent::toJSON();
    QJsonObject properties = json["properties"].toObject();
    properties.insert( "Escorrentía (%)" , CustomJsonExporter::format( QString::number( this->landuse_type->getRunoff(), 'f', 2 ) ) );
    json.insert( "properties" , properties );
    return json;
}

/**********************************************************************
 GETTERS
***********************************************************************/

LandUseType* LandUse::getLandUseType(){
    return this->landuse_type;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void LandUse::setLanduseType(LandUseType* landcover_type){
    this->landuse_type = landcover_type;
    this->setObjectName( landcover_type->getName() );
}
