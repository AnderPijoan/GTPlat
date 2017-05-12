#include "CustomJsonExporter.h"

#include <QJsonValue>
#include <QJsonArray>

#include "agents/Agent.h"
#include "utils/exporters/debugger/Debugger.h"
#include "utils/grid/GSSGrid.h"
#include "geos/geom/Envelope.h"
#include <QDebug>

/**********************************************************************
 SIMPLE ELEMENTS
**********************************************************************/

/**
 * @brief Creates the CUSTOM JSON format for attributes:
 *
 * properties {
 *  prop1 : {type : 'text' , value : 'Agent' , name : 'class'}
 *  prop2 : {...}
 * }
 *
 * @param value Property value
 * @return
 */
QJsonObject CustomJsonExporter::format( QVariant value ){
    QJsonObject json;
    switch( value.type() ){
    case QVariant::String:
        json.insert("type" , QString("text") );
        json.insert("value" , value.toString() );
        break;
    case QVariant::Int:
    case QVariant::UInt:
    case QVariant::Double:
    case QVariant::LongLong:
        json.insert("type" , QString("number") );
        json.insert("value" , value.toDouble() );
        break;
    case QVariant::Bool:
        json.insert("type" , QString("boolean") );
        json.insert("value" , value.toBool() );
        break;
    case QVariant::Color:
        json.insert("type" , QString("color") );
        json.insert("value" , value.toString() );
        break;
    case QVariant::DateTime:
    case QVariant::Date:
        json.insert("type" , QString("DateTime") );
        json.insert("value" , value.toDateTime().toMSecsSinceEpoch() );
        break;
    case QVariant::List: {
        QJsonArray values;
        foreach(QVariant v , value.toList()){
            QJsonObject json_value = CustomJsonExporter::format( v );
            values.append( json_value.value("value") );
            json.insert("type" , json_value.value("type") );
        }
        json.insert("values" , values );
        break;
    }
    default:
        Debugger::warning( 0 , Q_FUNC_INFO , QString("Unknown type") + QString( QVariant::typeToName( value.type() ) ) );
    }
    return json;
}

QJsonObject CustomJsonExporter::format(BaseUnit value ){
    return CustomJsonExporter::format( value.number() );
}


QJsonObject CustomJsonExporter::format(GSSGeometry* geom, QJsonObject properties ){
    QJsonObject json;
    json.insert("type" , QString("geo:json") );
    if( geom ){
        json.insert("value" , geom->toGeoJSON( properties ) );
    }
    return json;
}


/**********************************************************************
 AGENT
**********************************************************************/


QJsonObject CustomJsonExporter::format(Agent* agent ){
    QJsonObject json;
    json.insert("type" , QString("agent") );
    if( agent ){
        json.insert("value" , ((int) agent->getId()) );
    }
    return json;
}


/**********************************************************************
 GRID
**********************************************************************/


QJsonObject CustomJsonExporter::format( GSSGrid grid ){
    QJsonObject json;
    json.insert("type" , QString("geo:json") );

    QJsonObject grid_json;
    grid_json["type"] = "FeatureCollection";

    QJsonArray features;

    for(unsigned int x = 0; x < grid.getXSize(); x++){
        for(unsigned int y = 0; y < grid.getYSize(); y++){

            GSSEnvelope envelope = grid.getCellEnvelope( x, y );
            GSSCoordinate envelope_centroid = envelope.getCentroid();

            double value = grid.getValue( envelope_centroid );
            QJsonObject properties;

            if( value != NAN ){
                properties.insert( "value" , value );
                properties.insert( "color" , UiStyle::toHTML( QColor( 255 * ( 1 - ( ( grid.getMaxValue() - value ) / grid.getMaxValue() ) ) , 255 * ( ( grid.getMaxValue() - value ) / grid.getMaxValue() ) , 128  ) ) );
            }

            features.append( envelope.toGeoJSON( properties ) );
        }
    }
    grid_json["features"] = features;
    json.insert("value" , grid_json );

    return json;
}


/**********************************************************************
 MAP
**********************************************************************/

QJsonObject CustomJsonExporter::format( QMap<QString , int> map , QString map_title){
    QMap<QString , double> dmap;
    foreach(QString k , map.keys()){
        dmap.insert( k , map.value(k) );
    }
    return CustomJsonExporter::format( dmap , map_title );
}

QJsonObject CustomJsonExporter::format( QMap<QString , double> map , QString map_title){

    // https://github.com/krispo/angular-nvd3/blob/gh-pages/js/discreteBarChart.js

    QJsonObject json;
    json.insert("type" , QString("barchart") );

    QJsonObject map_json;

    // DATA
    QJsonArray values;
    foreach( QString key , map.keys() ){
        QJsonObject value;
        value.insert( "key" , key );
        value.insert( "value" , map.value( key ) );
        values.append( value );
    }

    QJsonObject values_with_title;
    values_with_title.insert( "key" , map_title );
    values_with_title.insert( "values" , values );

    map_json.insert( "data" , QJsonArray( {{values_with_title}} ) );

    json.insert( "value" , map_json );

    return json;
}
