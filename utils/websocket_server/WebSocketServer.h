#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QCoreApplication>
#include <QtWebSockets>
#include <QObject>
#include <QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtCore/QByteArray>

#include "environment/Environment.h"

class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    WebSocketServer(QCoreApplication* app, Environment* environment , unsigned int port = 3001);
    ~WebSocketServer();

public slots:
    void pushJson( QJsonObject json , QString operation );

private slots:
    void socketConnected();
    void socketDisconnected();

private:

    // METHODS
    void push(QByteArray bytes);

    QWebSocketServer* server;
    QList<QWebSocket*> connected_clients;
    Environment* environment;
    unsigned int port;
};

#endif // WEBSOCKETSERVER_H
