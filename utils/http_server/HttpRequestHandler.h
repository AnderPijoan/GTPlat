#ifndef HTTPREQUESTHANDLER_H
#define HTTPREQUESTHANDLER_H

#include <QUrl>
#include <QRegularExpressionMatch>
#include <QJsonObject>

#include "environment/Environment.h"

#include "libs/qhttp-master/src/qhttpserverrequest.hpp"
#include "libs/qhttp-master/src/qhttpserverresponse.hpp"

using namespace qhttp::server;

class HttpRequestHandler : public QObject
{
     Q_OBJECT

public:

    explicit HttpRequestHandler(QHttpRequest* req, QHttpResponse* res, Environment* environment);
    virtual ~HttpRequestHandler();

protected:

    void handleHttpRequest(QHttpRequest* req, QHttpResponse* res);

    Environment* environment;
    QByteArray body;

    // SERVER API
    void HTTP_Home(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );
    void HTTP_Static(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );

    // ENVIRONMENT API
    void HTTP_GetEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );
    void HTTP_UpdateEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );
    void HTTP_RunEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );
    void HTTP_StopEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );

    // AGENT API
    void HTTP_GetAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );
    void HTTP_RunAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );
    void HTTP_StopAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );

    void HTTP_CreateOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );
    void HTTP_GetOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );
    void HTTP_UpdateOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );
    void HTTP_RunOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );
    void HTTP_StopOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );
    void HTTP_DeleteOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body = QJsonObject() );

    // PRIVATES
    void HTTP_WriteResponse( QHttpResponse *res , Agent* agent , QString operation = "UPDATE");
    void HTTP_WriteResponse( QHttpResponse *res , QList<Agent*> agents , QString operation = "UPDATE" , int total_count = -1);

};

#endif // HTTPREQUESTHANDLER_H
