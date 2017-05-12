#ifndef TILESERVER_H
#define TILESERVER_H

#include <QCoreApplication>

#include "environment/Environment.h"

#include "libs/qhttp-master/src/qhttpserver.hpp"
#include "utils/tile_server/TileRequestHandler.h"

using namespace qhttp::server;

class TileServer : public QObject
{
    Q_OBJECT

public:
    TileServer( QCoreApplication* app , Environment* environment , int port = 3000 );
    ~TileServer();

protected:
    QHttpServer* server;
    Environment* environment;
};

#endif // TILESERVER_H
