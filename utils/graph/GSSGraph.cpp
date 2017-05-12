#include "GSSGraph.h"

#include "environment/Environment.h"
#include "geos/geom/CoordinateArraySequenceFactory.h"
#include "geos/geom/Envelope.h"

GSSGraph::GSSGraph() : PlanarGraph(){
    this->nodes_index = new GSSQuadtree();
}

GSSGraph::~GSSGraph() {
    delete this->nodes_index;
}

/**********************************************************************
 GETTERS
**********************************************************************/

GSSGraphEdge* GSSGraph::findEdge(GSSCoordinate from, GSSCoordinate to) const{
    if( from == to ){
        return 0;
    }
    GSSGraphNode* from_node = this->findNode( from );
    GSSGraphNode* to_node = this->findNode( to );
    if( !from_node || !to_node ){
        return 0;
    }
    for(unsigned int i = 0; i < this->dirEdges.size(); i++){
        GSSGraphEdge* edge = dynamic_cast<GSSGraphEdge*>( this->dirEdges.at(i) );
        if( edge->getFromNode() == from_node && edge->getToNode() == to_node ){
            return edge;
        }
    }
    return 0;
}

GSSGraphNode* GSSGraph::findNode(GSSCoordinate coor) const{
    geos::planargraph::Node* n = const_cast<GSSGraph*>( this )->PlanarGraph::findNode( geos::geom::Coordinate( coor.getX() , coor.getY() , coor.getZ() ) );
    if( n ){
        return dynamic_cast<GSSGraphNode*>( n );
    }
    return 0;
}

/** NOTICE that first of all it will call the FindNode method
 * @brief Graph::findNearestNode
 * @param coor
 * @return
 */
GSSGraphNode* GSSGraph::findNearestNode(GSSCoordinate coor) const{
    GSSGraphNode* found = this->findNode( coor );
    if( !found ){
        GSSEnvelope env = GSSEnvelope( coor.getX() , coor.getX() , coor.getY() , coor.getY() );
        QList<void *> objs = this->nodes_index->getElements( env );

        if( !objs.isEmpty() ){
            found = ( (GSSGraphNode*) objs.at( qrand() % objs.size() ) );
            GSSCoordinate found_coor = found->getCoordinate();
            double found_distance = coor.distance( found_coor );
            for(int i = 0 ; i < objs.size() ; i++){
                GSSGraphNode* g = ((GSSGraphNode*) objs.at(i));
                if( g ){
                    GSSCoordinate g_coor = g->getCoordinate();
                    double d = coor.distance( g_coor );
                    if( d < found_distance ){
                        found = g;
                        found_distance = d;
                    }
                }
            }
        }
    }
    return found;
}

QList<GSSGraphEdge*> GSSGraph::getEdges() const{
    QList<GSSGraphEdge*> edges;
    for(unsigned int i = 0; i < this->dirEdges.size(); i++){
        edges.append( dynamic_cast<GSSGraphEdge*>( this->dirEdges.at(i) ) );
    }
    return edges;
}

QList<GSSGraphEdge*> GSSGraph::getEdgesInEnvelope(GSSEnvelope env) const{
    QList<GSSGraphEdge*> edges;
    for(unsigned int i = 0; i < this->dirEdges.size(); i++){
        geos::planargraph::DirectedEdge* e  = dynamic_cast<geos::planargraph::DirectedEdge*>( this->dirEdges.at(i) );
        geos::geom::Coordinate c2 = e->getToNode()->getCoordinate();
        geos::geom::Coordinate c1 = e->getFromNode()->getCoordinate();
        if( env.intersects( GSSEnvelope( GSSCoordinate( c1.x , c1.y ) , GSSCoordinate( c2.x , c2.y ) ) ) ){
            edges.append( dynamic_cast<GSSGraphEdge*>( this->dirEdges.at(i) )  );
        }
    }
    return edges;
}

QList<GSSGraphNode*> GSSGraph::getNodes() const{
    QList<GSSGraphNode*> nodes;
    std::vector<geos::planargraph::Node*> ns;
    const_cast<GSSGraph*>( this )->PlanarGraph::getNodes( ns );
    for(unsigned int i = 0; i < ns.size(); i++){
        nodes.append( dynamic_cast<GSSGraphNode*>( ns.at(i) ) );
    }
    return nodes;
}

QMap<GSSGraphEdge*,double> GSSGraph::getCostMap() const{
    QMap<GSSGraphEdge*,double> cost_map;
    foreach( GSSGraphEdge* edge , this->getEdges() ){
        cost_map.insert( edge , edge->getCost() );
    }
    return cost_map;
}

QList<GSSGraphNode*> GSSGraph::findNodesOfDegree( int degree ) const{
    QList<GSSGraphNode*> nodes;
    std::vector<geos::planargraph::Node*> ns;
    const_cast<GSSGraph*>( this )->PlanarGraph::findNodesOfDegree( degree , ns );
    for(unsigned int i = 0; i < ns.size(); i++){
        nodes.append( dynamic_cast<GSSGraphNode*>( ns.at(i) ) );
    }
    return nodes;
}

int GSSGraph::countNodes() const{
    return this->getNodes().size();
}

int GSSGraph::countEdges() const{
    return this->dirEdges.size();
}

MultiLineString* GSSGraph::toMultiLineString( GeometryFactory* factory ) const{
    std::vector<Geometry*> *lines = new std::vector<Geometry*>();
    foreach( GSSGraphEdge* e , this->getEdges() ){
        std::size_t s = 0;
        CoordinateSequence* seq = factory->getCoordinateSequenceFactory()->create( s , s );
        seq->add( geos::geom::Coordinate ( e->getFromNode()->getCoordinate().getX() , e->getFromNode()->getCoordinate().getY() , e->getFromNode()->getCoordinate().getZ() ) );
        seq->add(  geos::geom::Coordinate ( e->getToNode()->getCoordinate().getX() , e->getToNode()->getCoordinate().getY() , e->getToNode()->getCoordinate().getZ() ) );
        geos::geom::Geometry* line = factory->createLineString( seq );
        lines->push_back( line );
    }
    return factory->createMultiLineString( lines );
}

/**********************************************************************
 SETTERS
**********************************************************************/

void GSSGraph::addEdge(GSSGraphEdge *edge){
    GSSGraphNode* from = this->findNode( edge->getFromNode()->getCoordinate() );
    if( !from ){
        this->addNode( edge->getFromNode() );
    }
    GSSGraphNode* to = this->findNode( edge->getToNode()->getCoordinate() );
    if( !to ){
        this->addNode( edge->getToNode() );
    }
    geos::planargraph::PlanarGraph::add( edge );
}

void GSSGraph::removeEdge(GSSGraphEdge *edge){
    auto it = std::find( this->dirEdges.begin(), this->dirEdges.end(), edge);
    if(it != this->dirEdges.end()){
        this->dirEdges.erase( it );
    }
    //geos::planargraph::PlanarGraph::remove( edge ); NOT WORKING
}

void GSSGraph::addNode(GSSGraphNode *node){
    GSSEnvelope env = GSSEnvelope( node->getCoordinate().getX() , node->getCoordinate().getX() , node->getCoordinate().getY() , node->getCoordinate().getY() );
    this->nodes_index->insert( env , node );
    geos::planargraph::PlanarGraph::add( node );
}

void GSSGraph::removeNode(GSSGraphNode *node){
    GSSEnvelope env = GSSEnvelope( node->getCoordinate().getX() , node->getCoordinate().getX() , node->getCoordinate().getY() , node->getCoordinate().getY() );
    this->nodes_index->remove( env , node );
    geos::planargraph::PlanarGraph::remove( node );
}

void GSSGraph::replaceNode(GSSGraphNode* old_node , GSSGraphNode *new_node){

    // Take out connected edges
    foreach( GSSGraphEdge* e , old_node->getDepartingEdges() ){
        this->removeEdge( e );
    }
    foreach( GSSGraphEdge* e , old_node->getArrivingEdges() ){
        this->removeEdge( e );
    }

    // Remove old node
    this->removeNode( old_node );
    // Add new node
    this->addNode( new_node );

    // Add again edges with the new node set
    foreach( GSSGraphEdge* e , old_node->getDepartingEdges() ){
        e->from = new_node;
        this->addEdge( e );
    }
    foreach( GSSGraphEdge* e , old_node->getArrivingEdges() ){
        e->to = new_node;
        this->addEdge( e );
    }
}

