#ifndef GSSDIJKSTRAROUTINGGRAPH_H
#define GSSDIJKSTRAROUTINGGRAPH_H

#include "utils/routing/GSSRoutingGraph.h"

#include <lemon/dijkstra.h>

/**
 * @brief The DijkstraRoutingGraph class WARNING!! TAKES OWNERSHIP OF GRAPH
 */
class GSSDijkstraRoutingGraph : public GSSRoutingGraph
{
    Q_OBJECT

public:
    GSSDijkstraRoutingGraph( QMap<GSSGraphEdge*,double> cost_map );

    // METHODS
    QList<GSSGraphEdge*> dijkstraShortestPath(GSSGraphNode* from, GSSGraphNode* to );
    QList<QList<GSSGraphEdge*> > dijkstraShortestPath(QList<GSSGraphNode*> ordered_nodes );
    QList<QList<GSSGraphEdge*> > dijkstraShortestPaths(GSSGraphNode* from_node, QList<GSSGraphNode*> to_nodes );
    GSSGraphNode* dijkstraNearestNode(GSSGraphNode* from_node, QList<GSSGraphNode*> to_nodes );

};

#endif // GSSDIJKSTRAROUTINGGRAPH_H
