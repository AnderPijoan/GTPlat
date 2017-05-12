#include "GSSDijkstraRoutingGraph.h"

#include "utils/exporters/debugger/Debugger.h"
#include <lemon/bellman_ford.h>
#include <lemon/path.h>

GSSDijkstraRoutingGraph::GSSDijkstraRoutingGraph( QMap<GSSGraphEdge*,double> cost_map ) : GSSRoutingGraph( cost_map ){

}

/**********************************************************************
 METHODS
**********************************************************************/

/**
 * @brief DijkstraRoutingGraph::dijkstraShortestPath Given the ordered nodes to go through, calculates the entire route
 * @param edges
 * @param go_through_nodes
 * @return
 */
QList<QList<GSSGraphEdge*> > GSSDijkstraRoutingGraph::dijkstraShortestPath(QList<GSSGraphNode*> ordered_nodes ){
    QList<QList<GSSGraphEdge*> > result_routes;

    Dijkstra<ListDigraph, ListDigraph::ArcMap<double> > dijkstra_graph( *this->routing_graph , *this->routing_graph_costs );

    for(int i = 0; i < ordered_nodes.size()-1; i++){

        QList<GSSGraphEdge*> result_route;
        GSSGraphNode* from_node = ordered_nodes.at( i );
        GSSGraphNode* to_node = ordered_nodes.at( i+1 );

        if( !from_node || !to_node ){
            Debugger::info( this , Q_FUNC_INFO , QString("Start (%1) or end node (%2) is emtpy").arg( from_node->getCoordinate().toString() ).arg( to_node->getCoordinate().toString() ) );
            result_routes.append( result_route );
            continue;
        }

        if( from_node->getCoordinate() == to_node->getCoordinate() ){
            Debugger::info( this , Q_FUNC_INFO , QString("Same start and end nodes (%1)").arg( from_node->getCoordinate().toString() ) );
            result_routes.append( result_route );
            continue;
        }

        GSSCoordinate start_node_coor = from_node->getCoordinate();
        GSSCoordinate end_node_coor = to_node->getCoordinate();

        if ( !this->node_coordinates.keys().contains( start_node_coor ) || !this->node_coordinates.keys().contains( end_node_coor ) ){
            Debugger::info( this , Q_FUNC_INFO , QString("Start (%1) or end node (%2) are not in graph").arg( start_node_coor.toString() ).arg( end_node_coor.toString() ) );
            result_routes.append( result_route );
            continue;
        }

        // Compute dijkstra shortest path
        ListDigraph::Node start = this->node_coordinates.value( start_node_coor );
        ListDigraph::Node end = this->node_coordinates.value( end_node_coor );

        if( !dijkstra_graph.run( start , end ) ){
            Debugger::info( this , Q_FUNC_INFO , QString("Can not reach end node (%2) from start (%1)").arg( from_node->getCoordinate().toString() ).arg( to_node->getCoordinate().toString() ) );
            result_routes.append( result_route );
            continue;
        }

        // Get route
        Path<ListDigraph> shortest_path = dijkstra_graph.path( end );
        for(int i = 0 ; i < shortest_path.length() ; i++) {
            ListDigraph::Arc arc = shortest_path.nth( i );
            result_route.append( this->arc_to_original_edges.value( arc ) );
        }
        result_routes.append( result_route );
    }

    return result_routes;
}

/**
 * @brief DijkstraRoutingGraph::dijkstraShortestPath Given a star and a list of end nodes calculates all the routes
 * @param edges
 * @param start_node
 * @param end_nodes
 * @return
 */
QList<QList<GSSGraphEdge*> > GSSDijkstraRoutingGraph::dijkstraShortestPaths(GSSGraphNode *from_node, QList<GSSGraphNode *> to_nodes ){
    QList<QList<GSSGraphEdge*> > result_routes;

    // Get start node and start graph from it
    GSSCoordinate start_node_coor = from_node->getCoordinate();
    ListDigraph::Node start = this->node_coordinates.value( start_node_coor );

    Dijkstra<ListDigraph, ListDigraph::ArcMap<double> > dijkstra_graph( *this->routing_graph , *this->routing_graph_costs );
    dijkstra_graph.run( start );

    // Iterate all end nodes
    foreach( GSSGraphNode* node , to_nodes ){
        QList<GSSGraphEdge*> route;

        GSSCoordinate node_coor = node->getCoordinate();
        ListDigraph::Node n = this->node_coordinates.value( node_coor );

        Path<ListDigraph> shortest_path = dijkstra_graph.path( n );
        for(int i = 0 ; i < shortest_path.length() ; i++) {
            ListDigraph::Arc arc = shortest_path.nth( i );
            route.append( this->arc_to_original_edges.value( arc ) );
        }

        result_routes.append( route );
    }

    return result_routes;
}

/**
 * @brief DijkstraRoutingGraph::dijkstraShortestPath Given a start and end node, calculates the route
 * @param edges
 * @param start_node
 * @param end_node
 * @return
 */
QList<GSSGraphEdge*> GSSDijkstraRoutingGraph::dijkstraShortestPath(GSSGraphNode* from_node, GSSGraphNode* to_node ){

    QList<GSSGraphNode*> nodes;
    nodes.append( from_node );
    nodes.append( to_node );

    return this->dijkstraShortestPath(nodes).at(0);
}

/**
 * @brief DijkstraRoutingGraph::dijkstraNearestNode Given a set of GraphNodes, calculates the routing nearest one
 * @param edges Edges to create the graph
 * @param nodes
 * @return
 */
GSSGraphNode* GSSDijkstraRoutingGraph::dijkstraNearestNode(GSSGraphNode* from_node, QList<GSSGraphNode*> to_nodes ){

    GSSGraphNode* result_node = 0;
    LengthUnit min_length( std::numeric_limits<double>::max() );

    QList<QList<GSSGraphEdge*> > routes = this->dijkstraShortestPaths(from_node , to_nodes);

    if( routes.size() != to_nodes.size() ){
        Debugger::warning( this , Q_FUNC_INFO , "Node list and routes size do not match" );
    }

    for(int node_pos = 0; node_pos < routes.size() && node_pos < to_nodes.size() ; node_pos++){
        LengthUnit c;

        foreach( GSSGraphEdge* edge , routes.at(node_pos) ){
            c = c + edge->getLength();
        }

        if( c <= min_length ){
            min_length = c;
            result_node = to_nodes.at( node_pos );
        }
    }

    return result_node;
}
