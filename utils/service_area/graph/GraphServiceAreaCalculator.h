#ifndef GRAPHSERVICEAREACALCULATOR_H
#define GRAPHSERVICEAREACALCULATOR_H

#include "utils/service_area/ServiceAreaCalculator.h"
#include "utils/graph/GSSGraph.h"

#include "utils/geometry/GSSGeometryController.h"
#include "geos/geom/Geometry.h"

using namespace geos::geom;

class GraphServiceAreaCalculator : public ServiceAreaCalculator
{
    Q_OBJECT

public:
    GraphServiceAreaCalculator( QObject* parent = 0 );

    // METHODS
    QList<const GSSGraphEdge*> getReachableEdges( const GSSGraph* graph , const GSSGraphNode* from , double cost );

private:
    void getReachableEdgesRecursive( const GSSGraph* graph , const GSSGraphNode* from , double cost , QList<const GSSGraphNode*>* visited_nodes , QList<const GSSGraphEdge*>* reachable_edges );
};

#endif // GRAPHSERVICEAREACALCULATOR_H
