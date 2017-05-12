#include "GSSGraphEdge.h"

#include <QDebug>
#include "utils/graph/GSSGraphNode.h"

/**
 * WARNING!! IF START NODE AND END NODE ARE THE SAME, CREATING A GRAPHEDGE WILL CRASH
 */
GSSGraphEdge::GSSGraphEdge(GSSGraphNode* start_node, GSSGraphNode* end_node) : geos::planargraph::DirectedEdge( start_node , end_node , end_node->getCoordinate().inner_coordinate , true ){
    if( !start_node || !end_node ){
        qWarning() << Q_FUNC_INFO << "Edge created without start or end node";
        return;
    }
    this->from = start_node;
    this->to = end_node;

    // Connect
    dynamic_cast<GSSGraphNode*>( this->from )->connectEdge( this );
    dynamic_cast<GSSGraphNode*>( this->to )->connectEdge( this );

    // Magic number 110574
    this->cost = this->from->getCoordinate().distance( this->to->getCoordinate() ) * 110574;
    this->length = LengthUnit( this->cost );
}

GSSGraphEdge::GSSGraphEdge(GSSGraphEdge *first_edge, GSSGraphEdge *second_edge) : GSSGraphEdge( first_edge->getFromNode() , second_edge->getToNode() ){
    this->setLength( first_edge->getLength() + second_edge->getLength() );
}

GSSGraphEdge::~GSSGraphEdge(){
    // Disconnect
    dynamic_cast<GSSGraphNode*>( this->from )->disconnectEdge( this );
    dynamic_cast<GSSGraphNode*>( this->to )->disconnectEdge( this );
}

/**********************************************************************
 GETTERS
**********************************************************************/

GSSGraphNode* GSSGraphEdge::getFromNode() const{
    return dynamic_cast<GSSGraphNode*>( geos::planargraph::DirectedEdge::getFromNode() );
}

GSSGraphNode* GSSGraphEdge::getToNode() const{
    return dynamic_cast<GSSGraphNode*>( geos::planargraph::DirectedEdge::getToNode() );
}

LengthUnit GSSGraphEdge::getLength() const {
    return this->length;
}

double GSSGraphEdge::getCost() const{
    return this->cost;
}

bool GSSGraphEdge::equals( const GSSGraphEdge *other) const{
    return this->getFromNode()->equals( other->getFromNode() ) && this->getToNode()->equals( other->getToNode() );
}

bool GSSGraphEdge::equalsReversed(const GSSGraphEdge *other) const{
    return this->getFromNode()->equals( other->getToNode() ) && this->getToNode()->equals( other->getFromNode() );
}

/**********************************************************************
 SETTERS
**********************************************************************/

void GSSGraphEdge::setLength(LengthUnit length){
    this->length = length;
}
