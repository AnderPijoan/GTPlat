#ifndef DYNAMICARCMAP_H
#define DYNAMICARCMAP_H

#include <QObject>
#include <QDebug>

#include <lemon/core.h>
#include <lemon/list_graph.h>
#include <lemon/maps.h>

using namespace lemon;

template <typename _Value>

class DynamicArcMap : public MapExtender<DefaultMap<ListDigraph, ListDigraph::Arc, _Value> > {

    typedef MapExtender<DefaultMap<ListDigraph, ListDigraph::Arc, _Value> > Parent;

public:

    explicit DynamicArcMap(const ListDigraph& digraph) : Parent(digraph) {}
    DynamicArcMap(const ListDigraph& digraph, const _Value& value) : Parent(digraph, value) {}

private:

    DynamicArcMap& operator=(const DynamicArcMap& cmap) {
        qDebug() << 1;
        return operator=<DynamicArcMap>(cmap);
    }

    template <typename CMap>
    DynamicArcMap& operator=(const CMap& cmap) {
        qDebug() << 2;
        Parent::operator=(cmap);
        return *this;
    }

};

#endif // DYNAMICARCMAP_H
