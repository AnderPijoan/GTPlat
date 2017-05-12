#include <QThread>

#include "HttpServer.h"

#include "utils/exporters/debugger/Debugger.h"
#include "libs/qhttp-master/src/qhttpserverrequest.hpp"
#include "libs/qhttp-master/src/qhttpserverresponse.hpp"

HttpServer::HttpServer( QCoreApplication* app, Environment* environment , int port ) : QObject( app )
{

    this->environment = environment;
    this->server = new QHttpServer();

    this->server->listen(QHostAddress::Any , port , [&](QHttpRequest* req, QHttpResponse* res){ // listening on 0.0.0.0:port
        // On each request, create a RequestHandler that will be deleted on request end
        new HttpRequestHandler(req, res, this->environment);
    });

    if ( this->server->isListening() ){
         Debugger::info( this , Q_FUNC_INFO , QString("HttpServer initiated in port %1").arg( port ) );
    } else {
         Debugger::warning( this , Q_FUNC_INFO , QString("HttpServer could not be initiated in port %1").arg( port ) );
    }
}

HttpServer::~HttpServer(){
    this->server->deleteLater();
}
