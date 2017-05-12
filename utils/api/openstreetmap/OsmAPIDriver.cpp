#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>

#include "OsmAPIDriver.h"

#include "geos/geom/CoordinateSequenceFactory.h"
#include "geos/geom/Point.h"
#include "geos/geom/LineString.h"

OsmAPIDriver::OsmAPIDriver(const GSSGeometryController *geometry_controller, QObject *parent) : QObject(parent){
    this->geometry_controller = geometry_controller;
}

OsmAPIDriver::~OsmAPIDriver(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

/**
  WARNING! Caller takes responsability to delete geometries it doesnt want to use
 * @brief OsmAPIDriver::downloadFromOsm
 * @param bounds
 * @param tags
 * @param nodes
 * @param ways
 * @param relations
 * @return
 */
QList<OsmImporter::OsmElement> OsmAPIDriver::getOsmElements( Geometry *bounds, QMap<QString, QString> tags,  bool nodes, bool ways, bool relations){

    //http://wiki.openstreetmap.org/wiki/Overpass_API/Language_Guide

    QString bbox = "(poly:\"";
    for(unsigned int i = 0; i < bounds->getCoordinates()->size() ; i++){
        Coordinate coor = bounds->getCoordinates()->getAt(i);
        if( i > 0 ){ bbox += " "; }
        bbox += QString("%1 %2")
                .arg( coor.y )
                .arg( coor.x );
    }
    bbox += "\")";

    QString query = "[out:xml][timeout:250]; (";


    // NODES
    if( nodes ){
        query += "node";

        foreach(QString key , tags.keys()){
            query += "[\"" + key + "\"";
            if( !tags.value(key).isEmpty() ){
                query += "=\"" + tags.value(key) + "\"";
            }
            query += "]";
        }

        query += bbox  + ";";
    }

    // WAYS
    if( ways ){
        query += "way";


        foreach(QString key , tags.keys()){
            query += "[\"" + key + "\"";
            if( !tags.value(key).isEmpty() ){
                query += "=\"" + tags.value(key) + "\"";
            }
            query += "]";
        }

        query += bbox  + ";";
    }

    // RELATIONS
    if( relations ){
        query += "relation";

        foreach(QString key , tags.keys()){
            query += "[\"" + key + "\"";
            if( !tags.value(key).isEmpty() ){
                query += "=\"" + tags.value(key) + "\"";
            }
            query += "]";
        }

        query += bbox  + ";";
    }

    query += ");out body; >; out skel qt;";

    QUrlQuery qurl_query;
    qurl_query.addQueryItem("data" , query);

    QMap<QString, QString> headers;
    headers.insert( "application" , "json" );

    QList<OsmImporter::OsmElement> elements;

    SyncAPIDriver* api = new SyncAPIDriver( this );
    QNetworkReply* reply = api->POST( QUrl("https://overpass-api.de/api/interpreter") , headers , qurl_query.toString(QUrl::FullyEncoded).toUtf8() );
    api->deleteLater();

    // If no reply
    if( !reply ){
        return elements;
    }

    // If the request returned an error
    if( reply->error() != QNetworkReply::NoError ){
        qWarning() << Q_FUNC_INFO << "Network error" << reply->errorString() << endl;
        reply->deleteLater();
        return elements;
    }

    // Else create OSM temporary file and parse it
    QFile temporary_file( QDir::currentPath() + "/" + "osm_api_temp");
    if( temporary_file.open( QIODevice::WriteOnly ) ){

        temporary_file.write( reply->readAll() );
        temporary_file.close();

        OsmImporter* importer = new OsmImporter( temporary_file.fileName() );

        foreach(OsmImporter::OsmElement e , importer->getOsmElements( this->geometry_controller , nodes , ways , relations ) ){
            elements.append( e );
        }

        importer->deleteLater();
        QFile::remove( temporary_file.fileName() );
    }

    reply->deleteLater();

    return elements;
}
