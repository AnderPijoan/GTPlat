#include "PublicTransportUseRoutingSkill.h"

#include "environment/Environment.h"
#include "utils/graph/GSSGraph.h"
#include "utils/routing/GSSDijkstraRoutingGraph.h"
#include "agents/transport_networks/walk_paths/WalkPath.h"

PublicTransportUseRoutingSkill::PublicTransportUseRoutingSkill(Agent *agent, QString walkpath_graph_class_name, QString public_transport_graph_class_name) : RoutingSkill( agent , walkpath_graph_class_name ){
    this->public_transport_graph_class_name = public_transport_graph_class_name;
}

/**********************************************************************
 GETTERS
**********************************************************************/

QList<PublicTransportStretch*> PublicTransportUseRoutingSkill::getRemainingPublicTransportStretchs(){
    return this->remaining_public_transport_stretchs;
}

/**********************************************************************
 METHODS
**********************************************************************/

void PublicTransportUseRoutingSkill::reloadGraph(){

    // Check if SharedObjectStorage has the graph we need
    this->routing_graph = dynamic_cast<GSSDijkstraRoutingGraph*>( SharedObjectStorage::getInstance()->getByName( QString("%1:%2").arg( GSSDijkstraRoutingGraph::staticMetaObject.className() ).arg( this->graph_class_name ) ) );
    if( this->routing_graph ){ return; }

    // Get walk paths
    const GSSGraph* walk_graph = this->skilled_agent->getEnvironment()->getNetworkEnvironment()->getGraph( this->graph_class_name );

    // Get public transport stretchs
    const GSSGraph* public_transport_graph = this->skilled_agent->getEnvironment()->getNetworkEnvironment()->getGraph( this->public_transport_graph_class_name );

    if( !walk_graph ){
        Debugger::info( this , Q_FUNC_INFO , "No walking graph found, returning empty route");
        return;
    }

    QMap<GSSGraphEdge*,double> cost_map;
    foreach(GSSGraphEdge* e , walk_graph->getEdges()){
        WalkPath* w = dynamic_cast<WalkPath*>( e );
        if( !w->isPermanentlyClosed() || w->isAllowed( this->skilled_agent ) ){
            cost_map.insert( e , e->getCost() );
        }
    }
    foreach(GSSGraphEdge* e , public_transport_graph->getEdges()){
        cost_map.insert( e , e->getCost() );
    }

    GSSDijkstraRoutingGraph* dijkstra_graph = new GSSDijkstraRoutingGraph( cost_map );
    dijkstra_graph->setObjectName( QString("%1:%2").arg( GSSDijkstraRoutingGraph::staticMetaObject.className() ).arg( this->graph_class_name ) );

    // Store by its name for other skills to reuse it
    SharedObjectStorage::getInstance()->add( dijkstra_graph );
    this->routing_graph = dijkstra_graph;
}


void PublicTransportUseRoutingSkill::calculateRoute( GSSCoordinate from, GSSCoordinate to){

    this->remaining_public_transport_stretchs.clear();
    this->route_edges.clear();

    if( from == to ){
        return;
    }

    // Check if we have the graph we need
    if( !this->routing_graph ){ this->reloadGraph(); }

    if( !this->routing_graph ){
        Debugger::info( this , Q_FUNC_INFO , "No graph found, returning empty route");
        return;
    }

    GSSGraphNode* from_node = this->routing_graph->findNearestNode( from );
    GSSGraphNode* to_node = this->routing_graph->findNearestNode( to );
    if( !from_node || !to_node ){
        Debugger::info( this , Q_FUNC_INFO , "No start or end points, returning empty route");
        return;
    }
    if( from_node == to_node ){
        Debugger::info( this , Q_FUNC_INFO , "Equal points, returning empty route");
        return;
    }

    this->current_edge_index = -1;
    this->route_edges = this->routing_graph->dijkstraShortestPath( from_node , to_node );

    foreach( GSSGraphEdge* e , this->route_edges ){
        PublicTransportStretch* using_pt = dynamic_cast<PublicTransportStretch*>( e );
        if( using_pt ){
            this->remaining_public_transport_stretchs.append( using_pt );
        }
    }
}
