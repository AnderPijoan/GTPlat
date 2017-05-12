#ifndef GSSGRAPHNODE_H
#define GSSGRAPHNODE_H

#include <QList>

#include "geos/geom/Coordinate.h"
#include "geos/planargraph/Node.h"
#include "geos/planargraph/PlanarGraph.h"

#include "utils/geometry/GSSCoordinate.h"
#include "utils/graph/GSSGraph.h"

QT_FORWARD_DECLARE_CLASS(GSSGraphEdge)

using namespace geos::geom;
using namespace geos::planargraph;

class GSSGraphNode : public geos::planargraph::Node
{
    friend class GSSGraph;
    friend class GSSGraphEdge;

public:
    GSSGraphNode( GSSCoordinate coor );

    // GETTERS
    virtual const QList<GSSGraphEdge*> getDepartingEdges() const;
    virtual const QList<GSSGraphEdge*> getArrivingEdges() const;
    virtual const GSSGraphEdge* getDepartingEdgeTo( GSSGraphNode* to ) const;
    virtual const GSSGraphEdge* getArrivingEdgeFrom( GSSGraphNode* from ) const;
    virtual const GSSCoordinate getCoordinate() const;
    virtual size_t getDegree() const;

    // SETTERS

    // METHODS
    bool equals(GSSGraphNode* other);

private:
    void connectEdge(GSSGraphEdge* edge);
    void disconnectEdge(GSSGraphEdge* edge);

    QList<GSSGraphEdge*> out_edges; // Departing edges
    QList<GSSGraphEdge*> in_edges; // Arriving edges
};

#endif // GSSGRAPHNODE_H
