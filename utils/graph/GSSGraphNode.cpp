#include "GSSGraphNode.h"

#include <QDebug>

#include "utils/graph/GSSGraphEdge.h"

GSSGraphNode::GSSGraphNode( GSSCoordinate coor ) : geos::planargraph::Node( coor.inner_coordinate ){
    this->pt = coor.inner_coordinate;
}

/**********************************************************************
 GETTERS
**********************************************************************/

const QList<GSSGraphEdge*> GSSGraphNode::getDepartingEdges() const{
    return this->out_edges;
}

const QList<GSSGraphEdge*> GSSGraphNode::getArrivingEdges() const{
    return this->in_edges;
}

const GSSGraphEdge* GSSGraphNode::getDepartingEdgeTo(GSSGraphNode *to) const{
    foreach(GSSGraphEdge* e , this->getDepartingEdges() ){
        if( e->getToNode()->equals( to ) ){
            return e;
        }
    }
    return 0;
}

const GSSGraphEdge* GSSGraphNode::getArrivingEdgeFrom(GSSGraphNode *from) const{
    foreach(GSSGraphEdge* e , this->getArrivingEdges() ){
        if( e->getFromNode()->equals( from ) ){
            return e;
        }
    }
    return 0;
}

const GSSCoordinate GSSGraphNode::getCoordinate() const{
    return GSSCoordinate( this->pt.x , this->pt.y , this->pt.z );
}

size_t GSSGraphNode::getDegree() const{
    size_t degree = this->out_edges.size();

    // Compare the directed opposite edges as one
    foreach( GSSGraphEdge* edge , this->getArrivingEdges() ){
        bool add = true;
        foreach( GSSGraphEdge* edge2 , this->getDepartingEdges() ){
            if( edge2->equalsReversed( edge ) ){
                add = false;
                break;
            }
        }
        if( add ){ degree++; }
    }
    return degree;
}

/**********************************************************************
 PRIVATE
**********************************************************************/

void GSSGraphNode::connectEdge(GSSGraphEdge *edge){
    if( edge->getFromNode() == this && !this->getDepartingEdges().contains( edge ) ){
        geos::planargraph::Node::addOutEdge( edge );
        this->out_edges.append( edge );
    }
    if( edge->getToNode() == this &&  !this->getArrivingEdges().contains( edge )){
        this->in_edges.append( edge );
    }
}

void GSSGraphNode::disconnectEdge(GSSGraphEdge *edge){
    if( edge->getFromNode() == this ){
        geos::planargraph::Node::getOutEdges()->remove( edge );
        this->out_edges.removeAll( edge );
    }
    if( edge->getToNode() == this ){
        this->in_edges.removeAll( edge );
    }
}

/**********************************************************************
 METHODS
**********************************************************************/

bool GSSGraphNode::equals(GSSGraphNode* other){
    if( !other ){
        return false;
    }
    return this->getCoordinate() == other->getCoordinate();
}

