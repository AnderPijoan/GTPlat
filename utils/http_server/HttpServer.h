#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QCoreApplication>

#include "environment/Environment.h"

#include "libs/qhttp-master/src/qhttpserver.hpp"
#include "utils/http_server/HttpRequestHandler.h"

using namespace qhttp::server;

class HttpServer : public QObject
{
    Q_OBJECT
public:

    HttpServer( QCoreApplication* app , Environment* environment , int port = 3000 );
    ~HttpServer();

protected:
    QHttpServer* server;
    Environment* environment;
};

#endif // HTTPSERVER_H
