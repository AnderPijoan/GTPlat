#include "TSPRoutingGraph.h"

#include "lemon/dijkstra.h"
#include "lemon/nearest_neighbor_tsp.h"
#include "lemon/christofides_tsp.h"
#include "lemon/greedy_tsp.h"
#include "lemon/insertion_tsp.h"
#include "lemon/opt2_tsp.h"

TSPRoutingGraph::TSPRoutingGraph( QMap<GSSGraphEdge*,double> cost_map, QList<GSSGraphNode *> visit_nodes) : GSSRoutingGraph( cost_map ){

    // Create distance between all visit nodes as graph
    QList< ListDigraph::Node > travel_graph_nodes;

    foreach( GSSGraphNode* node , visit_nodes ){
        GSSCoordinate node_coor = node->getCoordinate();

        if ( this->node_coordinates.keys().contains( node_coor ) ){
            travel_graph_nodes.append( this->node_coordinates.value( node_coor ) );
        } else {
            qWarning() << QString("[TSPRoutingGraph:TSPRoutingGraph] One visit node is not in graph, it will be discarded.") << endl;
        }
    }

    // Create distance_matrix (it automatically creates nodes and edges with ids from 0)
    this->distance_matrix = new FullGraph( travel_graph_nodes.size() );
    // Create costmap
    this->distance_matrix_weights = new FullGraph::EdgeMap<double>( *this->distance_matrix );

    // Iterate distance_matrix nodes and link them to network nodes
    for(int i = 0; i < this->distance_matrix->nodeNum(); i++){
        this->graph_matrix_nodes.insert( travel_graph_nodes.at(i) , this->distance_matrix->nodeFromId(i) );
    }

    // Create routing graph and calculate all to all distances
    Dijkstra<ListDigraph, ListDigraph::ArcMap<double> > dijkstra_graph( *this->routing_graph , *this->routing_graph_costs );

    // Iterate all edges in distance matrix and calculate their costs
    for(int i = 0; i < this->graph_matrix_nodes.size(); i++ ){
        for(int j = i+1; j < this->graph_matrix_nodes.size(); j++ ){

            FullGraph::Node from = this->graph_matrix_nodes.values().at(i);
            FullGraph::Node to = this->graph_matrix_nodes.values().at(j);
            FullGraph::Edge edge = this->distance_matrix->edge( from , to );

            // Calculate routing distance
            dijkstra_graph.run( this->graph_matrix_nodes.key( from ) , this->graph_matrix_nodes.key( to ) );
            double distance = dijkstra_graph.dist( this->graph_matrix_nodes.key( to ) );

            this->distance_matrix_weights->set( edge , distance );
        }
    }

}

TSPRoutingGraph::~TSPRoutingGraph(){
    // Delete created stuff
    delete this->distance_matrix;
    delete this->distance_matrix_weights;
}

/**********************************************************************
 METHODS
**********************************************************************/

QList<GSSGraphNode*> TSPRoutingGraph::nearestNeighborTsp(GSSGraphNode* start_node, GSSGraphNode* end_node){

    NearestNeighborTsp< FullGraph::EdgeMap<double> >* tsp = new NearestNeighborTsp< FullGraph::EdgeMap<double> >( *this->distance_matrix , *this->distance_matrix_weights );
    tsp->run();
    QList<GSSGraphNode*> tsp_circular_route_nodes;
    foreach( FullGraph::Node n , tsp->tourNodes() ){
        tsp_circular_route_nodes.append( this->node_to_original_node.value( this->graph_matrix_nodes.key( n ) ) );
    }

    return this->orderCircularTsp(start_node , end_node , tsp_circular_route_nodes);
}

QList<GSSGraphNode*> TSPRoutingGraph::greedyTsp(GSSGraphNode* start_node, GSSGraphNode* end_node){

    GreedyTsp< FullGraph::EdgeMap<double> >* tsp = new GreedyTsp< FullGraph::EdgeMap<double> >( *this->distance_matrix , *this->distance_matrix_weights );
    tsp->run();
    QList<GSSGraphNode*> tsp_circular_route_nodes;
    foreach( FullGraph::Node n , tsp->tourNodes() ){
        tsp_circular_route_nodes.append( this->node_to_original_node.value( this->graph_matrix_nodes.key( n ) ) );
    }

    return this->orderCircularTsp(start_node , end_node , tsp_circular_route_nodes);
}

QList<GSSGraphNode*> TSPRoutingGraph::insertionTsp(GSSGraphNode* start_node, GSSGraphNode* end_node){

    InsertionTsp< FullGraph::EdgeMap<double> >* tsp = new InsertionTsp< FullGraph::EdgeMap<double> >( *this->distance_matrix , *this->distance_matrix_weights );
    tsp->run();
    QList<GSSGraphNode*> tsp_circular_route_nodes;
    foreach( FullGraph::Node n , tsp->tourNodes() ){
        tsp_circular_route_nodes.append( this->node_to_original_node.value( this->graph_matrix_nodes.key( n ) ) );
    }

    return this->orderCircularTsp(start_node , end_node , tsp_circular_route_nodes);
}

QList<GSSGraphNode*> TSPRoutingGraph::christofidesTsp(GSSGraphNode* start_node, GSSGraphNode* end_node){

    InsertionTsp< FullGraph::EdgeMap<double> >* tsp = new InsertionTsp< FullGraph::EdgeMap<double> >( *this->distance_matrix , *this->distance_matrix_weights );
    tsp->run();
    QList<GSSGraphNode*> tsp_circular_route_nodes;
    foreach( FullGraph::Node n , tsp->tourNodes() ){
        tsp_circular_route_nodes.append( this->node_to_original_node.value( this->graph_matrix_nodes.key( n ) ) );
    }

    return this->orderCircularTsp(start_node , end_node , tsp_circular_route_nodes);
}

QList<GSSGraphNode*> TSPRoutingGraph::opt2Tsp(GSSGraphNode* start_node, GSSGraphNode* end_node){

    InsertionTsp< FullGraph::EdgeMap<double> >* tsp = new InsertionTsp< FullGraph::EdgeMap<double> >( *this->distance_matrix , *this->distance_matrix_weights );
    tsp->run();
    QList<GSSGraphNode*> tsp_circular_route_nodes;
    foreach( FullGraph::Node n , tsp->tourNodes() ){
        tsp_circular_route_nodes.append( this->node_to_original_node.value( this->graph_matrix_nodes.key( n ) ) );
    }

    return this->orderCircularTsp(start_node , end_node , tsp_circular_route_nodes);
}

/**********************************************************************
 PRIVATE
**********************************************************************/

/**
 * @brief TSPRoutingGraph::orderCircularTsp TSP algorithms return the shortest circular route between all through nodes
 * but don't tell from which node start and end. Order the circular route here
 * @param start_node
 * @param end_node
 * @param tsp_circular_nodes
 * @return
 */
QList<GSSGraphNode*> TSPRoutingGraph::orderCircularTsp(GSSGraphNode* start_node, GSSGraphNode* end_node, QList<GSSGraphNode*> tsp_circular_nodes){
    Q_UNUSED(end_node)

    QList<GSSGraphNode*> tsp_ordered_nodes;

    // Find the nearest circular_route node to start_node
    GSSGraphNode* nearest = tsp_circular_nodes.at( 0 );
    double min_distance = start_node->getCoordinate().distance( nearest->getCoordinate() );
    foreach(GSSGraphNode* n , tsp_circular_nodes){
        double d = n->getCoordinate().distance( start_node->getCoordinate() );
        if( min_distance > d ){
            nearest = n;
            min_distance = d;
        }
    }

    // Now order the circular route starting from the nearest node
    int nearest_index = tsp_circular_nodes.indexOf( nearest );
    for(int i = 0; i < tsp_circular_nodes.size() ; i++){
        tsp_ordered_nodes.append( tsp_circular_nodes.at( (nearest_index + i) % tsp_circular_nodes.size() ) );
    }

    return tsp_ordered_nodes;
}
