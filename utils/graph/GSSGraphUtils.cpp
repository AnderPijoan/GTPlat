#include "GSSGraphUtils.h"

#include "utils/exporters/debugger/Debugger.h"

GSSGraphUtils* GSSGraphUtils::getInstance(){
    static GSSGraphUtils instance;
    return &instance;
}

GSSGraphUtils::GSSGraphUtils(QObject *parent) : QObject(parent){
}

GSSGraphUtils::~GSSGraphUtils(){
}

/**********************************************************************
 METHODS
**********************************************************************/

/**
 * WARNING Caller gets ownership ONLY of created simplified version of GSSGraphEdges
 * @brief GSSGraphUtils::createSimplification
 * @param cost_map
 * @return
 */
QMap<GSSGraphEdge* , QList<GSSGraphEdge*> > GSSGraphUtils::createSimplification(QMap<GSSGraphEdge*,double> cost_map){

    QMap<GSSGraphEdge* , QList<GSSGraphEdge*> > simplified_to_real; // QMAP< SIMPLIFIED_EDGE , QLIST< ORIGINAL EDGES > >
    QMap<GSSGraphNode* , GSSGraphNode*> old_to_new_nodes;

    QList<GSSGraphEdge*> used_edges;
    foreach( GSSGraphEdge* edge , cost_map.keys() ){

        if( used_edges.contains(edge) ){
            continue;
        }

        // Find an edge starting in an intersection/dead end
        if( edge->getFromNode()->getDegree() != 2 ){

            // If degree == 1; starts in a dead end
            // If degree > 2; starts in an intersection

            // Equivalence list
            QList<GSSGraphEdge*> new_edge_to_old_edges;

            // Iterate edge until arriving at degree != 2 ToNode
            GSSGraphNode* from = edge->getFromNode();
            GSSGraphEdge* current_edge = edge;

            GSSGraphNode* to = current_edge->getToNode();
            double acc_cost = cost_map.value( current_edge , current_edge->getCost() );
            LengthUnit acc_length = current_edge->getLength();
            new_edge_to_old_edges.append( current_edge );

            // While degree == 2; Not dead end nor intersection node
            while( current_edge->getToNode()->getDegree() == 2 ){

                // Get next edge, that doesnt return back to the start point
                GSSGraphEdge* next_edge = 0;
                foreach( GSSGraphEdge* e , current_edge->getToNode()->getDepartingEdges() ){

                    // Should only be 1 (if one way direction) or 2 (if double direction)
                    // Choose the one that doesnt return back to our start point
                    if( !e->getToNode()->equals( current_edge->getFromNode() ) ){
                        next_edge = e;
                    }
                }

                // If there is no next_edge, dead end. Finish loop
                if( !next_edge ){ break; }

                current_edge = next_edge;
                to = current_edge->getToNode();
                acc_cost += cost_map.value( current_edge , current_edge->getCost() );
                acc_length = acc_length + current_edge->getLength();
                new_edge_to_old_edges.append( current_edge );
            }

            // Create new nodes, not to connect the new edges to real nodes
            GSSGraphNode* new_from = old_to_new_nodes.value( from , 0 );
            if( !new_from ){
                new_from = new GSSGraphNode( from->getCoordinate() );
            }
            GSSGraphNode* new_to = old_to_new_nodes.value( to , 0 );
            if( !new_to ){
                new_to = new GSSGraphNode( to->getCoordinate() );
            }

            // If from and to are the same, do not create edge
            if( !new_from->equals( new_to ) ){

                GSSGraphEdge* new_edge = new GSSGraphEdge( new_from , new_to );
                new_edge->cost = acc_cost;
                new_edge->setLength( acc_length );
                simplified_to_real.insert( new_edge , new_edge_to_old_edges );
            }

            used_edges.append( new_edge_to_old_edges );
        }
    }

    Debugger::info( this , Q_FUNC_INFO , QString("Simplified %1 into %2 edges").arg( used_edges.size() ).arg( simplified_to_real.size() ) );

    return simplified_to_real;
}
