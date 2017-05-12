#ifndef GSSGRAPH_H
#define GSSGRAPH_H

#include "geos/planargraph/PlanarGraph.h"
#include "geos/geom/MultiLineString.h"
#include "geos/geom/GeometryFactory.h"

#include "utils/geometry/GSSQuadtree.h"
#include "utils/graph/GSSGraphNode.h"
#include "utils/graph/GSSGraphEdge.h"
#include "utils/geometry/GSSCoordinate.h"

using namespace geos::planargraph;

class GSSGraph : public PlanarGraph
{
public:
    explicit GSSGraph();
    ~GSSGraph();

    // GETTERS
    GSSGraphNode* findNode( GSSCoordinate coor ) const;
    GSSGraphNode* findNearestNode( GSSCoordinate coor ) const;
    GSSGraphEdge* findEdge( GSSCoordinate from , GSSCoordinate to ) const;
    QList<GSSGraphNode*> findNodesOfDegree( int degree ) const;
    QList<GSSGraphNode*> getNodes() const;
    QList<GSSGraphEdge*> getEdges() const;
    QList<GSSGraphEdge*> getEdgesInEnvelope( GSSEnvelope ) const;
    QMap<GSSGraphEdge*, double> getCostMap() const;
    int countNodes() const;
    int countEdges() const;
    MultiLineString* toMultiLineString( GeometryFactory* factory ) const;

    // SETTERS
    void replaceNode( GSSGraphNode* old_node , GSSGraphNode* new_node );
    void addNode( GSSGraphNode* node );
    void addEdge( GSSGraphEdge* edge );
    void removeNode( GSSGraphNode* node );
    void removeEdge( GSSGraphEdge* edge );

private:
    GSSQuadtree* nodes_index;
};

#endif // GSSGRAPH_H
