#ifndef TSPROUTINGGRAPH_H
#define TSPROUTINGGRAPH_H

#include "utils/routing/GSSRoutingGraph.h"

#include <lemon/core.h>
#include <lemon/list_graph.h>
#include <lemon/full_graph.h>

class TSPRoutingGraph : public GSSRoutingGraph
{
    Q_OBJECT

public:

    TSPRoutingGraph( QMap<GSSGraphEdge*,double> cost_map , QList<GSSGraphNode *> visit_nodes );
    ~TSPRoutingGraph();

    // METHODS
    QList<GSSGraphNode*> nearestNeighborTsp(GSSGraphNode* start_node, GSSGraphNode* end_node);
    QList<GSSGraphNode*> greedyTsp(GSSGraphNode* start_node, GSSGraphNode* end_node);
    QList<GSSGraphNode*> insertionTsp(GSSGraphNode* start_node, GSSGraphNode* end_node);
    QList<GSSGraphNode*> christofidesTsp(GSSGraphNode* start_node, GSSGraphNode* end_node);
    QList<GSSGraphNode*> opt2Tsp(GSSGraphNode* start_node, GSSGraphNode* end_node);

private:
    // Distance matrix represented as a graph
    FullGraph* distance_matrix;
    // Distance matrix graph costs
    FullGraph::EdgeMap<double>* distance_matrix_weights;

    // Map to relate graph and distance matrix nodes
    QMap<ListDigraph::Node , FullGraph::Node> graph_matrix_nodes;

    QList<GSSGraphNode*> orderCircularTsp(GSSGraphNode* start_node, GSSGraphNode* end_node, QList<GSSGraphNode*> tsp_circular_nodes);

};

#endif // TSPROUTINGGRAPH_H
