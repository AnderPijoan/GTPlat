#ifndef GSSROUTINGGRAPH_H
#define GSSROUTINGGRAPH_H

#include <QObject>

#include <lemon/core.h>
#include <lemon/list_graph.h>

#include <geos/geom/Geometry.h>
#include <geos/geom/Coordinate.h>

#include "utils/routing/DynamicArcMap.h"
#include "utils/graph/GSSGraph.h"
#include "utils/graph/GSSGraphEdge.h"
#include "utils/graph/GSSGraphNode.h"

using namespace geos::geom;
using namespace lemon;

class GSSRoutingGraph : public QObject
{
    Q_OBJECT

public:
    GSSRoutingGraph( QMap<GSSGraphEdge*,double> cost_map );
    ~GSSRoutingGraph();

    // GETTERS
    GSSGraphNode* findNearestNode( const GSSCoordinate coor );

protected:

    // Graph
    ListDigraph* routing_graph;
    // Costs
    ListDigraph::ArcMap<double>* routing_graph_costs;
    // Node coordinate map to avoid duplicates
    QMap<GSSCoordinate, ListDigraph::Node> node_coordinates;
    // Node and Edge arc map to retrieve them
    QMap<ListDigraph::Arc, GSSGraphEdge*> arc_to_simplified_edge; // QMAP< LISTDIGRAPH ARC ,SIMPLIFIED EDGE >
    QMap<ListDigraph::Arc, QList<GSSGraphEdge*> > arc_to_original_edges; // QMAP< LISTDIGRAPH ARC , QLIST< ORIGINAL EDGES> >
    QMap<ListDigraph::Node, GSSGraphNode*> node_to_original_node;

private:
    GSSGraph* inner_simplified_graph; // Inner simplified graph in order to be able to find nearest nodes

};

#endif // GSSROUTINGGRAPH_H
