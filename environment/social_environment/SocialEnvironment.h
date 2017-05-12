#ifndef SOCIALENVIRONMENT_H
#define SOCIALENVIRONMENT_H

#include "agents/Agent.h"

#include "utils/graph/GSSGraph.h"
#include "utils/graph/GSSGraphNode.h"
#include "utils/graph/GSSGraphEdge.h"

QT_FORWARD_DECLARE_CLASS(Environment)

using namespace geos::geom;
using namespace geos::planargraph;

class SocialEnvironment: public QObject
{
    Q_OBJECT
    friend class Environment; // Environment can access the constructor

private:
    explicit SocialEnvironment(Environment* parent);
    ~SocialEnvironment();

    // ENVIRONMENT METHODS
    bool addAgent( Agent* agent );
    bool removeAgent( Agent* agent );

private:

    QMap<QString, GSSGraph*> spatial_graph; // Social graphs

};

#endif // SOCIALENVIRONMENT_H
