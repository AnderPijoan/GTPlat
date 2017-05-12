#include "NetworkEnvironment.h"

#include "environment/Environment.h"

NetworkEnvironment::NetworkEnvironment(Environment *parent) : QObject(parent){

}

NetworkEnvironment::~NetworkEnvironment(){
    qDeleteAll(this->network_graphs.values());
}

/**********************************************************************
 GETTERS
**********************************************************************/

GSSGraphNode* NetworkEnvironment::getNodeFromGraph( GSSCoordinate coor, QString class_name) const{
    if( !this->network_graphs.keys().contains( class_name ) ){
        return 0;
    }
    return this->network_graphs.value( class_name )->findNode( coor );
}

GSSGraphNode* NetworkEnvironment::getNearestNodeFromGraph( GSSCoordinate coor, QString class_name) const{
    if( !this->network_graphs.keys().contains( class_name ) ){
        return 0;
    }
    return this->network_graphs.value( class_name )->findNearestNode( coor );
}

const GSSGraphEdge* NetworkEnvironment::getEdgeFromGraph( GSSCoordinate from,  GSSCoordinate to, QString class_name) const{
    if( !this->network_graphs.keys().contains( class_name ) ){
        return 0;
    }
    return this->network_graphs.value( class_name )->findEdge( from , to );
}

const GSSGraph* NetworkEnvironment::getGraph( QString class_name ) const{
    if( !this->network_graphs.keys().contains( class_name ) ){
        return 0;
    }
    return this->network_graphs.value( class_name );
}

/**********************************************************************
 SETTERS
**********************************************************************/



/**********************************************************************
 PRIVATE
**********************************************************************/

bool NetworkEnvironment::addAgent(Agent *agent){

    GSSGraphEdge* edge = dynamic_cast<GSSGraphEdge*>( agent );
    GSSGraphNode* node = dynamic_cast<GSSGraphNode*>( agent );

    if( edge || node ){

        const QMetaObject* class_type = agent->metaObject();
        QList<QString> keys = this->network_graphs.keys();
        while( class_type && !keys.contains( class_type->className() ) ){

            // Insert new spatial graph with the agents class
            this->mutex.lock();
            GSSGraph* graph = new GSSGraph();
            this->network_graphs.insert( class_type->className() , graph );
            this->mutex.unlock();

            class_type = class_type->superClass();
        }

        class_type = agent->metaObject();
        while( class_type ){

            // Add to spatial graph
            if( edge ){
                this->network_graphs.value( class_type->className() )->addEdge( edge );
            }
            if( node ){
                this->network_graphs.value( class_type->className() )->addNode( node );
            }

            class_type = class_type->superClass();
        }
    }

    return true;
}

bool NetworkEnvironment::updateAgent(Agent *agent){

    GSSGraphEdge* edge = dynamic_cast<GSSGraphEdge*>( agent );
    if( edge ){
        this->removeAgent( agent );
        this->addAgent( agent );
    }
    return true;
}

bool NetworkEnvironment::removeAgent(Agent *agent){

    const QMetaObject* obj = agent->metaObject();
    GSSGraphEdge* edge = dynamic_cast<GSSGraphEdge*>( agent );
    GSSGraphNode* node = dynamic_cast<GSSGraphNode*>( agent );
    while( (edge || node) && obj ){

        // Remove it from spatial graphs
        if( edge ){
            this->network_graphs.value( obj->className() )->removeEdge( edge );
        }
        if( node ){
            this->network_graphs.value( obj->className() )->removeNode( node );
        }

        obj = obj->superClass();
    }
    return true;
}
