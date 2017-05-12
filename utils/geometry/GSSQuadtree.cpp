#include "GSSQuadtree.h"

#include "geos/geom/Envelope.h"

GSSQuadtree::GSSQuadtree(QObject *parent) : QObject(parent){
    this->inner_index = new geos::index::quadtree::Quadtree();
}

GSSQuadtree::~GSSQuadtree(){
    delete this->inner_index;
    this->inner_index = 0;
}


QList<void *> GSSQuadtree::getElements(GSSEnvelope env){
    QList<void *> objs;
    std::vector<void*> vector;

    geos::geom::Envelope* e = new geos::geom::Envelope( env.getMinX() , env.getMaxX() , env.getMinY() , env.getMaxY() );
    this->inner_index->query( e , vector);
    delete e;

    if( vector.size() ){
        for(unsigned int i = 0 ; i < vector.size() ; i++){
            objs.append( vector.at(i) );
        }
    }
    return objs;
}

void GSSQuadtree::insert(GSSEnvelope env, void *elm){
    geos::geom::Envelope* e = new geos::geom::Envelope( env.getMinX() , env.getMaxX() , env.getMinY() , env.getMaxY() );
    this->inner_index->insert( e , elm );
    delete e;
}

void GSSQuadtree::remove(GSSEnvelope env , void *elm){
    geos::geom::Envelope* e = new geos::geom::Envelope( env.getMinX() , env.getMaxX() , env.getMinY() , env.getMaxY() );
    this->inner_index->remove( e , elm );
    delete e;
}
