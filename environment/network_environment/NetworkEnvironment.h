#ifndef NETWORKENVIRONMENT_H
#define NETWORKENVIRONMENT_H

#include <QObject>

#include "agents/Agent.h"
#include "utils/graph/GSSGraph.h"
#include "utils/graph/GSSGraphEdge.h"
#include "utils/graph/GSSGraphNode.h"

class NetworkEnvironment : public QObject
{
    Q_OBJECT
    friend class Environment; // Environment can access the constructor

public:
    QJsonObject toJSON();
    void fromJSON(QJsonObject json);

    // GETTERS
    GSSGraphNode* getNodeFromGraph( GSSCoordinate coor , QString class_name ) const;
    GSSGraphNode* getNearestNodeFromGraph( GSSCoordinate coor , QString class_name ) const;
    const GSSGraphEdge* getEdgeFromGraph( GSSCoordinate from , GSSCoordinate to , QString class_name ) const;
    const GSSGraph* getGraph( QString class_name ) const;

private:
    explicit NetworkEnvironment(Environment *parent);
    ~NetworkEnvironment();

    // AGENT METHODS
    bool addAgent( Agent* agent );
    bool updateAgent( Agent* agent );
    bool removeAgent( Agent* agent );

    QMap<QString, GSSGraph*> network_graphs; // Graphs

    // Mutex, for avoiding concurrency
    QMutex mutex;

};

#endif // NETWORKENVIRONMENT_H
