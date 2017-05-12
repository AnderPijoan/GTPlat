#include "GSSRoutingGraph.h"

#include "utils/exporters/debugger/Debugger.h"
#include "utils/graph/GSSGraphUtils.h"

/** WARNING, TAKES OWNERSHIP OF THE GIVEN GRAPH
 * @brief RoutingGraph::RoutingGraph
 * @param graph
 */
GSSRoutingGraph::GSSRoutingGraph( QMap<GSSGraphEdge*,double> cost_map ){

    if( cost_map.size() <= 0 ){
        Debugger::warning( this , Q_FUNC_INFO , "Empty cost map" );
        return;
    }

    // Create simplified version of the graph, joining all edges that are not connected to an intersection
    QMap<GSSGraphEdge* , QList<GSSGraphEdge*> > simplified_to_real_edges = GSSGraphUtils::getInstance()->createSimplification( cost_map );

    // Create graph and edge weights map
    this->inner_simplified_graph = new GSSGraph();
    this->routing_graph = new ListDigraph();
    this->routing_graph_costs = new ListDigraph::ArcMap<double>( *this->routing_graph );

    // Temporary list to fasten comparisons
    QList<GSSCoordinate> fast_compare_coords;

    // Create nodes and edges
    foreach( GSSGraphEdge* simplified_edge, simplified_to_real_edges.keys() ){

        try{

            this->inner_simplified_graph->addEdge( simplified_edge );

            // Create or retrieve edge start node
            GSSCoordinate start_node_coor = simplified_edge->getFromNode()->getCoordinate();
            ListDigraph::Node s;

            if( !fast_compare_coords.contains( start_node_coor ) ){
                s = this->routing_graph->addNode();
                this->node_coordinates.insert( start_node_coor , s );
                this->node_to_original_node.insert( s , simplified_edge->getFromNode() );
                fast_compare_coords.append( start_node_coor );
            } else {
                s = this->node_coordinates.value( start_node_coor );
            }

            // Create or retrieve edge end node
            GSSCoordinate end_node_coor = simplified_edge->getToNode()->getCoordinate();
            ListDigraph::Node e;

            if( !fast_compare_coords.contains( end_node_coor ) ){
                e = this->routing_graph->addNode();
                this->node_coordinates.insert( end_node_coor , e );
                this->node_to_original_node.insert( e , simplified_edge->getToNode() );
                fast_compare_coords.append( end_node_coor );
            } else {
                e = this->node_coordinates.value( end_node_coor );
            }

            // Create edge and its cost
            ListDigraph::Arc arc = this->routing_graph->addArc(s , e);

            // Create simplified edge to real edges map
            this->arc_to_simplified_edge.insert( arc , simplified_edge );
            this->arc_to_original_edges.insert( arc , simplified_to_real_edges.value( simplified_edge ));

            this->routing_graph_costs->set( arc , simplified_edge->getCost() );

        } catch(...){}
    }
}

GSSRoutingGraph::~GSSRoutingGraph(){
    // Delete created stuff
    qDeleteAll( this->arc_to_simplified_edge.values() ); // These are the simplified edges we created, so must be deleted by us
    delete this->inner_simplified_graph;
    delete this->routing_graph;
    delete this->routing_graph_costs;
}

/**********************************************************************
 GETTERS
**********************************************************************/

GSSGraphNode* GSSRoutingGraph::findNearestNode( const GSSCoordinate coor){
    return this->inner_simplified_graph->findNearestNode( coor );
}
