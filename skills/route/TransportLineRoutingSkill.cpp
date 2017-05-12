#include "TransportLineRoutingSkill.h"

#include "agents/vehicles/Vehicle.h"
#include "environment/Environment.h"

#include "skills/move/drive/DriveSkill.h"
#include "skills/view/ViewRouteAheadSkill.h"

TransportLineRoutingSkill::TransportLineRoutingSkill(Vehicle* vehicle , Agent *skilled_agent , QString graph_class_name ) : RoutingSkill( skilled_agent , graph_class_name ){
    this->vehicle = vehicle;
}

/**********************************************************************
 GETTERS
**********************************************************************/

TimeUnit TransportLineRoutingSkill::getRemainingTime(){
    TimeUnit time = TimeUnit( 0 );
    foreach(GSSGraphEdge* e , this->route_edges){
        TransportLine* t = dynamic_cast<TransportLine*>(e);
        if( t ){ time = time + ( t->getLength().number() / t->getMaxSpeed().number() ); }
    }
    return time;
}

Value TransportLineRoutingSkill::getRemainingCrossingPrice(){
    CurrencyUnit price = 0;
    foreach(GSSGraphEdge* e , this->route_edges){
        TransportLine* t = dynamic_cast<TransportLine*>(e);
        if( t ){ price = price + t->getCrossingPrice(); }
    }
    return price;
}

/**********************************************************************
 METHODS
**********************************************************************/

void TransportLineRoutingSkill::reloadGraph(){

    // Check if SharedObjectStorage has the graph we need
    this->routing_graph = dynamic_cast<GSSDijkstraRoutingGraph*>( SharedObjectStorage::getInstance()->getByName( QString("%1:%2").arg( GSSDijkstraRoutingGraph::staticMetaObject.className() ).arg( this->graph_class_name ) ) );
    if( this->routing_graph ){ return; }

    // If there is no vehicle
    if( !this->vehicle ){
        Debugger::warning( this , Q_FUNC_INFO , "Skill has no vehicle");
        return;
    }

    const GSSGraph* original_graph = this->skilled_agent->getEnvironment()->getNetworkEnvironment()->getGraph( this->graph_class_name );

    if( !original_graph ){
        Debugger::warning( this , Q_FUNC_INFO , "No graph found");
        return;
    }

    QMap<GSSGraphEdge*,double> cost_map;
    foreach(GSSGraphEdge* e , original_graph->getEdges()){
        TransportLine* t = dynamic_cast<TransportLine*>( e );
        if( t && !this->avoid_edges.contains( t ) && t->isAllowed( this->vehicle ) && !t->isPermanentlyClosed() ){
            cost_map.insert( t , t->getCost() );
        }
    }

    GSSDijkstraRoutingGraph* dijkstra_graph = new GSSDijkstraRoutingGraph( cost_map );
    dijkstra_graph->setObjectName( QString("%1:%2").arg( GSSDijkstraRoutingGraph::staticMetaObject.className() ).arg( this->graph_class_name ) );

    // Store by its name for other skills to reuse it
    SharedObjectStorage::getInstance()->add( dijkstra_graph );
    this->routing_graph = dijkstra_graph;
}

TransportLine* TransportLineRoutingSkill::getCurrentEdge(){
    return dynamic_cast<TransportLine*>( RoutingSkill::getCurrentEdge() );
}

TransportLine* TransportLineRoutingSkill::getFollowingEdge(){
    return dynamic_cast<TransportLine*>( RoutingSkill::getFollowingEdge() );
}

