#ifndef GSSGRAPHEDGE_H
#define GSSGRAPHEDGE_H

#include "geos/planargraph/DirectedEdge.h"
#include "geos/planargraph/PlanarGraph.h"

#include "utils/units/UnitConstants.h"
#include "utils/graph/GSSGraph.h"

QT_FORWARD_DECLARE_CLASS(GSSGraphNode)

using namespace geos::planargraph;

class GSSGraphEdge : public geos::planargraph::DirectedEdge
{
    friend class GSSGraph;
    friend class GSSGraphUtils;

public:
    GSSGraphEdge( GSSGraphNode* start_node, GSSGraphNode* end_node );
    GSSGraphEdge( GSSGraphEdge* first_edge, GSSGraphEdge* second_edge ); // Joins two edges ---FIRST---> ---SECOND--->
    ~GSSGraphEdge();

    // GETTERS
    virtual GSSGraphNode* getFromNode() const;
    virtual GSSGraphNode* getToNode() const;
    virtual LengthUnit getLength() const;
    virtual double getCost() const; // To be implemented by each child
    virtual bool equals( const GSSGraphEdge* other) const;
    virtual bool equalsReversed( const GSSGraphEdge* other ) const;

    // SETTERS
    void setLength(LengthUnit length);

private:

    LengthUnit length;
    double cost;

};

#endif // GSSGRAPHEDGE_H
