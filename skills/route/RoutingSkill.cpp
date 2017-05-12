#include "RoutingSkill.h"

#include "geos/geom/Coordinate.h"

#include "environment/Environment.h"
#include "environment/physical_environment/PhysicalEnvironment.h"
#include "utils/routing/GSSDijkstraRoutingGraph.h"

RoutingSkill::RoutingSkill(Agent* skilled_agent , QString graph_class_name ) : Skill(skilled_agent) , routing_graph( 0 ) {
    this->graph_class_name = graph_class_name;
}

RoutingSkill::~RoutingSkill(){
    // Do not delete routing graph as it is shared between others
}

/**********************************************************************
 GETTERS
**********************************************************************/

QList<GSSGraphEdge*> RoutingSkill::getRemainingRoute() const{
    QList<GSSGraphEdge*> l;
    for( int i = this->current_edge_index+1 ; i < this->route_edges.size() ; i++ ){
        l.append( this->route_edges.at(i) );
    }
    return l;
}

LengthUnit RoutingSkill::getRemainingLength() const{
    LengthUnit length;
    foreach(GSSGraphEdge* e , this->getRemainingRoute()){
        length = length + e->getLength();
    }
    return length;
}


/**********************************************************************
 METHODS
**********************************************************************/

void RoutingSkill::setRoute(QList<GSSGraphEdge *> route_edges){
    this->route_edges = route_edges;
}

void RoutingSkill::reloadGraph(){

    // Check if SharedObjectStorage has the graph we need
    this->routing_graph = dynamic_cast<GSSDijkstraRoutingGraph*>( SharedObjectStorage::getInstance()->getByName( QString("%1:%2").arg( GSSDijkstraRoutingGraph::staticMetaObject.className() ).arg( this->graph_class_name ) ) );
    if( this->routing_graph ){ return; }

    const GSSGraph* original_graph = this->skilled_agent->getEnvironment()->getNetworkEnvironment()->getGraph( this->graph_class_name );

    if( !original_graph ){
        Debugger::warning( this , Q_FUNC_INFO , "No graph found" );
        this->routing_graph = 0;
        return;
    }

    GSSDijkstraRoutingGraph* dijkstra_graph = new GSSDijkstraRoutingGraph( original_graph->getCostMap() );
    dijkstra_graph->setObjectName( QString("%1:%2").arg( GSSDijkstraRoutingGraph::staticMetaObject.className() ).arg( this->graph_class_name ) );

    // Store by its name for other skills to reuse it
    SharedObjectStorage::getInstance()->add( dijkstra_graph );
    this->routing_graph = dijkstra_graph;
}

void RoutingSkill::calculateRoute( GSSCoordinate from, GSSCoordinate to ){

    this->route_edges.clear();

    if( from == to ){
        Debugger::debug( this , Q_FUNC_INFO , "Already at route destination");
        this->skilled_agent->setNextTick();
        return;
    }

    if( !this->routing_graph ){ this->reloadGraph(); }

    if( !this->routing_graph ){
        Debugger::warning( this , Q_FUNC_INFO , "No graph found, returning empty route");
        return;
    }

    GSSGraphNode* from_node = this->routing_graph->findNearestNode( from );
    GSSGraphNode* to_node = this->routing_graph->findNearestNode( to );

    if( !from_node || !to_node ){
        Debugger::debug( this , Q_FUNC_INFO , "No start or end points, returning empty route" );
        return;
    }
    if( from_node == to_node ){
        Debugger::debug( this , Q_FUNC_INFO , "Equal points, returning empty route" );
        return;
    }

    this->current_edge_index = -1;
    this->route_edges = this->routing_graph->dijkstraShortestPath( from_node , to_node );
}

/**********************************************************************
 SLOTS
**********************************************************************/

GSSGraphEdge* RoutingSkill::getCurrentEdge(){
    if( this->current_edge_index < 0 || this->current_edge_index >= this->route_edges.size() ){
        return 0;
    }
    return this->route_edges.at( this->current_edge_index );
}

GSSGraphEdge* RoutingSkill::getFollowingEdge(){
    if( this->current_edge_index < -1 || this->current_edge_index > this->route_edges.size()-2 ){
        return 0;
    }
    return this->route_edges.at( this->current_edge_index+1 );
}

void RoutingSkill::pointToFollowingEdge(){
    this->current_edge_index++;
}
