#include "TileRequestHandler.h"

#include <QPainter>
#include <QImage>
#include <QBuffer>
#include <QJsonDocument>

#include "utils/geometry/GSSEnvelope.h"
#include "utils/heatmap/GSSHeatMap.h"
#include "utils/exporters/debugger/Debugger.h"
#include "utils/conversors/image_coordinates_conversor/ImageCoordinatesConversor.h"
#include "utils/conversors/tile_coordinates_conversor/TileCoordinatesConversor.h"

TileRequestHandler::TileRequestHandler(QHttpRequest* req, QHttpResponse* res, Environment* environment) : QObject() {

    Debugger::debug( this , Q_FUNC_INFO , QString("Request entered : [%1] %2").arg( req->method() ).arg( req->url().toString() ) );
    this->environment = environment;

    // Append chunks of data into uniform body.
    req->onData([this](const QByteArray& chunk) {
        this->body.append(chunk);
    });

    // When all the incoming data are gathered, send some response to client.
    req->onEnd([this, req, res]() {
        this->handleTileRequest( req , res );
    });
}

TileRequestHandler::~TileRequestHandler(){
}

/**********************************************************************
 METHODS
**********************************************************************/


void TileRequestHandler::handleTileRequest(QHttpRequest* req, QHttpResponse* res){

    // PNG TILE
    {
        QRegularExpressionMatch match = QRegularExpression("^/agents/(?<class_name>[A-Za-z0-9]+)/timestamp/(?<timestamp>[0-9]+)/tile/(?<z>[0-9]+)/(?<x>[0-9]+)/(?<y>[0-9]+).png$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_GET ){ this->agentPng( req , res , match ); return; }
    }

    // PNG TILE
    {
        QRegularExpressionMatch match = QRegularExpression("^/heatmaps/(?<class_name>[A-Za-z0-9]+)/timestamp/(?<timestamp>[0-9]+)/tile/(?<z>[0-9]+)/(?<x>[0-9]+)/(?<y>[0-9]+).png$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_GET ){ this->heatmapPng( req , res , match ); return; }
    }

    // JSON TILE
    {
        QRegularExpressionMatch match = QRegularExpression("^/agents/(?<class_name>[A-Za-z0-9]+)/timestamp/(?<timestamp>[0-9]+)/tile/(?<z>[0-9]+)/(?<x>[0-9]+)/(?<y>[0-9]+).json$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_GET ){ this->agentJson( req , res , match ); return; }
    }

    // GRID TILE
    {
        QRegularExpressionMatch match = QRegularExpression("^/grids/(?<class_name>[A-Za-z0-9]+)/timestamp/(?<timestamp>[0-9]+)/tile/(?<z>[0-9]+)/(?<x>[0-9]+)/(?<y>[0-9]+).png$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_GET ){ this->gridPng( req , res , match ); return; }
    }

    // GRAPH TILE
    {
        QRegularExpressionMatch match = QRegularExpression("^/graphs/(?<class_name>[A-Za-z0-9]+)/timestamp/(?<timestamp>[0-9]+)/tile/(?<z>[0-9]+)/(?<x>[0-9]+)/(?<y>[0-9]+).png$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_GET ){ this->graphPng( req , res , match ); return; }
    }

}

/**********************************************************************
 SERVER API
**********************************************************************/

void TileRequestHandler::agentPng(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req)
    Q_UNUSED(body)
    try {

        double top = TileCoordinatesConversor::tiley2lat( match.captured("y").toDouble() , match.captured("z").toDouble() );
        double bottom = TileCoordinatesConversor::tiley2lat( match.captured("y").toDouble() + 1 , match.captured("z").toDouble() );
        double left = TileCoordinatesConversor::tilex2lon( match.captured("x").toDouble() , match.captured("z").toDouble() );
        double right = TileCoordinatesConversor::tilex2lon( match.captured("x").toDouble() + 1 , match.captured("z").toDouble() );
        GSSEnvelope bounds = GSSEnvelope( left , right , bottom , top );

        QImage image = QImage( 512 , 512 , QImage::Format_ARGB32 );
        image.fill( QColor( 0 , 0 , 0 , 0 ) );

        QPainter painter( &image );
        painter.setRenderHint( QPainter::Antialiasing );

        foreach(Agent* a , this->environment->getPhysicalEnvironment()->getAgentsInsideEnvelope( bounds , match.captured("class_name") ) ){
            if( a && a->getStyle() && match.captured("z").toDouble() >= a->getStyle()->zoom_level ){
                painter.drawImage( 0 , 0 , a->toImage( bounds , image.width() , image.height() ) );
            }
        }

        // Transform QImage into QByteArray
        QByteArray filecontent;
        QBuffer buffer(&filecontent);
        buffer.open(QIODevice::WriteOnly);
        image.save( &buffer , "PNG" );

        res->setStatusCode( qhttp::ESTATUS_OK );
        res->addHeader("Content-Length", QByteArray::number( filecontent.size() ) );
        res->write( filecontent );

    } catch(...){}
    res->end();
}


void TileRequestHandler::heatmapPng(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req)
    Q_UNUSED(body)
    try {

        double top = TileCoordinatesConversor::tiley2lat( match.captured("y").toDouble() , match.captured("z").toDouble() );
        double bottom = TileCoordinatesConversor::tiley2lat( match.captured("y").toDouble() + 1 , match.captured("z").toDouble() );
        double left = TileCoordinatesConversor::tilex2lon( match.captured("x").toDouble() , match.captured("z").toDouble() );
        double right = TileCoordinatesConversor::tilex2lon( match.captured("x").toDouble() + 1 , match.captured("z").toDouble() );
        GSSEnvelope bounds = GSSEnvelope( left , right , bottom , top );

        GSSHeatMap heatmap;

        foreach(Agent* a , this->environment->getPhysicalEnvironment()->getAgentsInsideEnvelope( bounds , match.captured("class_name") ) ){
            if( match.captured("z").toDouble() >= a->getStyle()->zoom_level ){
                heatmap.addPoint( a->getRepresentativeCoordinate() );
            }
        }

        QImage image = heatmap.toImage( bounds , 512 , 512 );

        // Transform QImage into QByteArray
        QByteArray filecontent;
        QBuffer buffer(&filecontent);
        buffer.open(QIODevice::WriteOnly);
        image.save( &buffer , "PNG" );

        res->setStatusCode( qhttp::ESTATUS_OK );
        res->addHeader("Content-Length", QByteArray::number( filecontent.size() ) );
        res->write( filecontent );

    } catch(...){}
    res->end();
}

void TileRequestHandler::agentJson(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req)
    Q_UNUSED(body)
    try {

        double top = TileCoordinatesConversor::tiley2lat( match.captured("y").toDouble() , match.captured("z").toDouble() );
        double bottom = TileCoordinatesConversor::tiley2lat( match.captured("y").toDouble() + 1 , match.captured("z").toDouble() );
        double left = TileCoordinatesConversor::tilex2lon( match.captured("x").toDouble() , match.captured("z").toDouble() );
        double right = TileCoordinatesConversor::tilex2lon( match.captured("x").toDouble() + 1 , match.captured("z").toDouble() );
        GSSEnvelope bounds = GSSEnvelope( left , right , bottom , top );

        QVariantMap json;
        json.insert("type" , "FeatureCollection");
        QJsonArray json_agents;

        foreach(Agent* a , this->environment->getPhysicalEnvironment()->getAgentsInsideEnvelope( bounds , match.captured("class_name") ) ){
            if( match.captured("z").toDouble() >= a->getStyle()->zoom_level ){
                json_agents.append( a->getGeometry()->toGeoJSON() );
            }
        }

        json.insert( "features" , json_agents.toVariantList() );

        res->setStatusCode( qhttp::ESTATUS_OK );
        res->addHeader("Content-Type", "application/json");
        res->write( QJsonDocument::fromVariant( json ).toJson() );

    } catch(...){}
    res->end();
}

void TileRequestHandler::gridPng(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req)
    Q_UNUSED(body)

    try {

        double top = TileCoordinatesConversor::tiley2lat( match.captured("y").toDouble() , match.captured("z").toDouble() );
        double bottom = TileCoordinatesConversor::tiley2lat( match.captured("y").toDouble() + 1 , match.captured("z").toDouble() );
        double left = TileCoordinatesConversor::tilex2lon( match.captured("x").toDouble() , match.captured("z").toDouble() );
        double right = TileCoordinatesConversor::tilex2lon( match.captured("x").toDouble() + 1 , match.captured("z").toDouble() );
        GSSEnvelope env = GSSEnvelope(left , right , top , bottom);

        QImage image = QImage( 512 , 512 , QImage::Format_ARGB32 );
        image.fill( QColor( 0 , 0 , 0 , 0 ) );

        QPainter painter( &image );
        painter.setRenderHint( QPainter::Antialiasing );

        const GSSGrid grid = this->environment->getPhysicalEnvironment()->getGeometryController()->getElevationModel();
        painter.drawImage( 0 , 0 , grid.toImage( env , image.width() , image.height() ) );

        // Transform QImage into QByteArray
        QByteArray filecontent;
        QBuffer buffer(&filecontent);
        buffer.open(QIODevice::WriteOnly);
        image.save( &buffer , "PNG" );

        res->setStatusCode( qhttp::ESTATUS_OK );
        res->addHeader("Content-Length", QByteArray::number( filecontent.size() ) );
        res->write( filecontent );

    } catch(...){}
    res->end();
}

void TileRequestHandler::graphPng(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req)
    Q_UNUSED(body)

    const GSSGraph* graph = this->environment->getNetworkEnvironment()->getGraph( match.captured("class_name") );
    if( !graph ){
        res->end();
    }

    try {

        double top = TileCoordinatesConversor::tiley2lat( match.captured("y").toDouble() , match.captured("z").toDouble() );
        double bottom = TileCoordinatesConversor::tiley2lat( match.captured("y").toDouble() + 1 , match.captured("z").toDouble() );
        double left = TileCoordinatesConversor::tilex2lon( match.captured("x").toDouble() , match.captured("z").toDouble() );
        double right = TileCoordinatesConversor::tilex2lon( match.captured("x").toDouble() + 1 , match.captured("z").toDouble() );
        GSSEnvelope env = GSSEnvelope(left , right , top , bottom);

        QImage image = QImage( 512 , 512 , QImage::Format_ARGB32 );
        image.fill( QColor( 0 , 0 , 0 , 0 ) );

        QPainter painter( &image );
        painter.setRenderHint( QPainter::Antialiasing );
        QPen pen;
        pen.setWidth( 10 );
        painter.setPen( pen );

        foreach(GSSGraphEdge* e , graph->getEdgesInEnvelope( env ) ){

           QLine line = QLine(
                        ImageCoordinatesConversor::reprojectPoint(
                                                e->getFromNode()->getCoordinate().getY(),
                                                e->getFromNode()->getCoordinate().getX(),
                                                env.getMaxY(),
                                                env.getMinY(),
                                                env.getMaxX(),
                                                env.getMinX(),
                                                image.width(),
                                                image.height() ) ,
                        ImageCoordinatesConversor::reprojectPoint(
                                                e->getToNode()->getCoordinate().getY(),
                                                e->getToNode()->getCoordinate().getX(),
                                                env.getMaxY(),
                                                env.getMinY(),
                                                env.getMaxX(),
                                                env.getMinX(),
                                                image.width(),
                                                image.height() )
                       );

            painter.drawLine( line );
        }

        // Transform QImage into QByteArray
        QByteArray filecontent;
        QBuffer buffer(&filecontent);
        buffer.open(QIODevice::WriteOnly);
        image.save( &buffer , "PNG" );

        res->setStatusCode( qhttp::ESTATUS_OK );
        res->addHeader("Content-Length", QByteArray::number( filecontent.size() ) );
        res->end( filecontent );

    } catch(...){}
    res->end();
}

