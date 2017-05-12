#ifndef GSSQUADTREE_H
#define GSSQUADTREE_H

#include <QObject>

#include "utils/geometry/GSSEnvelope.h"
#include "geos/index/quadtree/Quadtree.h"

using namespace geos::index;

class GSSQuadtree : public QObject
{
    Q_OBJECT

public:
    explicit GSSQuadtree(QObject *parent = 0);
    ~GSSQuadtree();

    // GETTERS
    QList<void *> getElements( GSSEnvelope env );

    // SETTERS
    void insert( GSSEnvelope env , void * elm );
    void remove( GSSEnvelope env , void * elm );

private:
    geos::index::quadtree::Quadtree* inner_index;
};

#endif // GSSQUADTREE_H
