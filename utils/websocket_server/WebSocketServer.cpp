
#include "utils/exporters/debugger/Debugger.h"
#include <QJsonDocument>
#include <QVariantMap>
#include <QThreadPool>

#include "WebSocketServer.h"


WebSocketServer::WebSocketServer(QCoreApplication* app , Environment* environment , unsigned int port) : QObject( app ) {

    this->environment = environment;
    this->port = port;

    this->server = new QWebSocketServer(QStringLiteral("Echo Server"), QWebSocketServer::NonSecureMode);
    if (this->server->listen(QHostAddress::Any, this->port)) {
        Debugger::info( this , Q_FUNC_INFO ,  QString("Web Socket Server initiated in port %1").arg( this->port ) );

        QObject::connect(this->server , &QWebSocketServer::newConnection , this, &WebSocketServer::socketConnected );
        QObject::connect(this->server , &QWebSocketServer::closed , this , &WebSocketServer::socketDisconnected );

        // Connect to Environment signals
        QObject::connect( this->environment , &Environment::pushToInterfaceSignal , this , &WebSocketServer::pushJson );
    }
}

WebSocketServer::~WebSocketServer(){

    this->server->close();
    qDeleteAll( this->connected_clients.begin(), this->connected_clients.end() );

    Debugger::info( this , Q_FUNC_INFO ,  QString("Web Socket Server deleted") );
}

/**********************************************************************
 METHODS
**********************************************************************/


/**
 * @brief UI expects messages with format:
 *
 * {
 *  data : Agent
 *  operation : operation_code
 * }
 */
void WebSocketServer::pushJson(QJsonObject json, QString operation){

    QVariantMap json_element_and_operation;
    try {
        json_element_and_operation.insert("operation", operation);
        json_element_and_operation.insert("data", json.toVariantMap());
    } catch(...){}

    this->push( QJsonDocument::fromVariant( json_element_and_operation ).toJson() );
}

/**********************************************************************
 PRIVATE
**********************************************************************/


/**
 * @brief WebSocketServer::push Push through all connected websockets
 * @param bytes
 */
void WebSocketServer::push(QByteArray bytes){
    // Send to all connected clients
    foreach(QWebSocket* client , this->connected_clients){
        try {
            client->sendTextMessage( QString(bytes) );
        } catch(...){}
    }
}

/**********************************************************************
 SLOTS
**********************************************************************/

void WebSocketServer::socketConnected(){
    QWebSocket *socket = this->server->nextPendingConnection();
    if( socket && !this->connected_clients.contains( socket ) ){
        Debugger::info( this , Q_FUNC_INFO ,  QString("New socket connected %1").arg( socket->peerAddress().toString() ) );
        this->connected_clients.append( socket );
    }
}

void WebSocketServer::socketDisconnected(){
    QWebSocket* socket = qobject_cast<QWebSocket*>( sender() );
    if ( socket && this->connected_clients.contains( socket ) ) {
        Debugger::info( this , Q_FUNC_INFO ,  QString("Socket disconnected %1").arg( socket->peerAddress().toString() ) );
        this->connected_clients.removeAll( socket );
        socket->deleteLater();
    }
}


