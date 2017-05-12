#include "WalkPathRoutingSkill.h"

#include "environment/Environment.h"

WalkPathRoutingSkill::WalkPathRoutingSkill(Agent *skilled_agent , QString graph_class_name) : RoutingSkill( skilled_agent , graph_class_name ){
}

/**********************************************************************
 METHODS
**********************************************************************/

void WalkPathRoutingSkill::reloadGraph(){

    // Check if SharedObjectStorage has the graph we need
    this->routing_graph = dynamic_cast<GSSDijkstraRoutingGraph*>( SharedObjectStorage::getInstance()->getByName( QString("%1:%2").arg( GSSDijkstraRoutingGraph::staticMetaObject.className() ).arg( this->graph_class_name ) ) );
    if( this->routing_graph ){ return; }

    const GSSGraph* original_graph = this->skilled_agent->getEnvironment()->getNetworkEnvironment()->getGraph( this->graph_class_name );

    if( !original_graph ){
        Debugger::warning( this , Q_FUNC_INFO , "No graph found" );
        return;
    }

    QMap<GSSGraphEdge*,double> cost_map;
    foreach(GSSGraphEdge* e , original_graph->getEdges()){
        WalkPath* w = dynamic_cast<WalkPath*>( e );
        if( w && !w->isPermanentlyClosed() && w->isAllowed( this->skilled_agent ) ){
            cost_map.insert( w , w->getCost() );
        }
    }

    GSSDijkstraRoutingGraph* dijkstra_graph = new GSSDijkstraRoutingGraph( cost_map );
    dijkstra_graph->setObjectName( QString("%1:%2").arg( GSSDijkstraRoutingGraph::staticMetaObject.className() ).arg( this->graph_class_name ) );

    // Store by its name for other skills to reuse it
    SharedObjectStorage::getInstance()->add( dijkstra_graph );
    this->routing_graph = dijkstra_graph;
}

WalkPath* WalkPathRoutingSkill::getCurrentEdge(){
    return dynamic_cast<WalkPath*>( RoutingSkill::getCurrentEdge() );
}

WalkPath* WalkPathRoutingSkill::getFollowingEdge(){
    return dynamic_cast<WalkPath*>( RoutingSkill::getFollowingEdge() );
}
