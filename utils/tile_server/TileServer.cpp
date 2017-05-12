#include "TileServer.h"

#include "utils/exporters/debugger/Debugger.h"

TileServer::TileServer( QCoreApplication* app , Environment* environment , int port ) : QObject( app )
{

    this->environment = environment;
    this->server = new QHttpServer();

    this->server->listen(QHostAddress::Any , port , [&](QHttpRequest* req, QHttpResponse* res){
        // On each request, create a RequestHandler that will be deleted on request end
        new TileRequestHandler(req, res, this->environment);
    });

    if ( this->server->isListening() ){
        Debugger::info( this , Q_FUNC_INFO , QString("TileServer initiated in port %1").arg( port ) );
    } else {
        Debugger::warning( this , Q_FUNC_INFO , QString("TileServer could not be initiated in port %1").arg( port ) );
    }
}

TileServer::~TileServer(){
    this->server->deleteLater();
}
