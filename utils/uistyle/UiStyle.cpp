#include "UiStyle.h"

#include <QStringList>
#include <QJsonArray>
#include "utils/exporters/custom_json_exporter/CustomJsonExporter.h"

UiStyle::UiStyle(QObject *parent) : QObject(parent){
    this->border_weight = 4;
    this->color = QColor("Gray");
    this->border_color = QColor("Black");
    this->zoom_level = 0;
}

UiStyle::~UiStyle(){
}

QJsonObject UiStyle::toJSON(){
    QJsonObject style;
    style.insert( "color", CustomJsonExporter::format( this->color ) );
    style.insert( "fill_opacity", CustomJsonExporter::format( this->color.alpha() / double( 255 ) ) ); // To be between 0 and 1
    style.insert( "border_color", CustomJsonExporter::format( this->border_color ) );
    style.insert( "border_opacity", CustomJsonExporter::format( this->border_color.alpha() / double( 255 ) ) ); // To be between 0 and 1
    style.insert( "weight", CustomJsonExporter::format( this->border_weight ) );
    style.insert( "zoom_level", CustomJsonExporter::format( this->zoom_level ) );
    if( !this->dash_array.isEmpty() ) {
        style.insert( "dash_array", CustomJsonExporter::format( this->dash_array ) );
    }
    if( !this->icon_url.isEmpty() ) {
        style.insert( "icon_url", CustomJsonExporter::format( this->icon_url ) );
    }
    return style;
}

void UiStyle::fromJSON( QJsonObject json ){
    if( json.contains("color") ) { this->color = QColor( json["color"].toObject()["value"].toString() ); }
    if( json.contains("fill_opacity") ) { this->color.setAlpha( json["fill_opacity"].toObject()["value"].toString().toDouble() * 255 ); } // To be between 0 and 255
    if( json.contains("border_color") ) { this->border_color = QColor( json["border_color"].toObject()["value"].toString() ); }
    if( json.contains("border_opacity") ) { this->border_color.setAlpha( json["border_opacity"].toObject()["value"].toString().toDouble() * 255 ); } // To be between 0 and 255
    if( json.contains("weight") ) { this->border_weight = json["weight"].toObject()["value"].toString().toInt(); }
    if( json.contains("icon_url") ) { this->icon_url = json["icon_url"].toObject()["value"].toString(); }
    if( json.contains("dash_array") ) { this->dash_array = json["dash_array"].toObject()["value"].toString(); }
    if( json.contains("zoom_level") ) { this->zoom_level = json["zoom_level"].toObject()["value"].toInt(); }
}

/**********************************************************************
 METHODS
**********************************************************************/


QColor UiStyle::randomColor(){
    int rand = qrand() % QColor::colorNames().size();
    return QColor( QColor::colorNames().at( rand ) );
}

QString UiStyle::toHTML(QColor color){
    return color.name();
}
