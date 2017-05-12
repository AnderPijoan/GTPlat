#ifndef TRANSPORTNODE_H
#define TRANSPORTNODE_H

#include "utils/graph/GSSGraphNode.h"

QT_FORWARD_DECLARE_CLASS(TransportLine)

/**
 * @brief The TransportNode class : Usable to be connected to TransportLines. THIS IS NOT AN AGENT!!
 */
class TransportNode : public GSSGraphNode
{

public:
    TransportNode( GSSCoordinate coor );
};

#endif // TRANSPORTNODE_H
