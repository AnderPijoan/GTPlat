#include "GraphServiceAreaCalculator.h"

#include <QDebug>
#include "geos/geom/LineString.h"
#include "geos/simplify/TopologyPreservingSimplifier.h"

GraphServiceAreaCalculator::GraphServiceAreaCalculator(QObject *parent) : ServiceAreaCalculator( parent ){

}

/**********************************************************************
 GETTERS
**********************************************************************/

QList<const GSSGraphEdge*> GraphServiceAreaCalculator::getReachableEdges(const GSSGraph *graph, const GSSGraphNode *from, double cost){

    QList<const GSSGraphNode*>* visited_nodes_ref = new QList<const GSSGraphNode*>();
    QList<const GSSGraphEdge*>* reachable_edges_ref = new QList<const GSSGraphEdge*>();
    this->getReachableEdgesRecursive( graph , from , cost , visited_nodes_ref , reachable_edges_ref);

    QList<const GSSGraphEdge*> reachable_edges;
    foreach( const GSSGraphEdge* edge , *reachable_edges_ref ){
        reachable_edges.append( edge );
    }

    delete reachable_edges_ref;
    delete visited_nodes_ref;
    return reachable_edges;
}

/**********************************************************************
 PRIVATE
**********************************************************************/

/**
 * WARNING SOME EDGES MAY APPEAR MORE THAN ONCE IN THE RETURNED LIST
 * @brief GraphServiceAreaCalculator::getReachableEdges
 * @param graph
 * @param from
 * @param cost
 * @param visited_nodes
 * @return
 */
void GraphServiceAreaCalculator::getReachableEdgesRecursive(const GSSGraph *graph, const GSSGraphNode *from, double cost, QList<const GSSGraphNode *>* visited_nodes , QList<const GSSGraphEdge *>* reachable_edges){

    visited_nodes->append( from );
    foreach( GSSGraphEdge* edge , from->getDepartingEdges() ){

        double remaining_cost = cost - edge->getCost();
        GSSGraphNode* to_node = edge->getToNode();

        if( remaining_cost > 0 ){

            if( !reachable_edges->contains( edge ) ){ reachable_edges->append( edge ); }
            if( !visited_nodes->contains( to_node ) ){ this->getReachableEdgesRecursive( graph , to_node , remaining_cost , visited_nodes , reachable_edges ); }
        }
    }

}
