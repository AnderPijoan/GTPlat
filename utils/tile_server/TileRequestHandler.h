#ifndef TILEREQUESTHANDLER_H
#define TILEREQUESTHANDLER_H

#include <QUrl>
#include <QRegularExpressionMatch>
#include <QJsonObject>

#include "environment/Environment.h"
#include "utils/geometry/GSSGeometryController.h"

#include "libs/qhttp-master/src/qhttpserverrequest.hpp"
#include "libs/qhttp-master/src/qhttpserverresponse.hpp"

using namespace qhttp::server;

class TileRequestHandler : public QObject
{
    Q_OBJECT

public:

    explicit TileRequestHandler(QHttpRequest* req, QHttpResponse* res, Environment* environment);
    virtual ~TileRequestHandler();

protected:

    void handleTileRequest(QHttpRequest* req, QHttpResponse* res);

    Environment* environment;
    QByteArray body;

    // AGENTS AS PNG
    void agentPng(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );

    // AGENTS AS HEATMAP PNG
    void heatmapPng(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );

    // AGENTS AS GEOJSON
    void agentJson(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );

    // GRID AS PNG
    void gridPng(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );

    // GRAPH AS PNG
    void graphPng(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );


};

#endif // TILEREQUESTHANDLER_H
