#include "AgentStorage.h"

#include "agents/Agent.h"

AgentStorage::AgentStorage(QObject *parent) : ObjectStorage( parent ){

}

/**********************************************************************
 GETTERS
**********************************************************************/

Agent* AgentStorage::getByClassAndId(QString class_name, unsigned int id) const{
    if ( this->agents_memory_keys.contains( class_name ) ){
         return this->agents_memory[ class_name ].value( id , 0 );
    }
    return 0;
}

Agent* AgentStorage::getById(unsigned int id) const{
    foreach(QString class_name, this->agents_memory_keys){
        if( this->agents_memory[ class_name ].contains( id ) ){
            return this->agents_memory[ class_name ].value( id , 0 );
        }
    }
    return 0;
}

Agent* AgentStorage::getByClassAndName(QString class_name, QString name) const{
    return dynamic_cast<Agent*>( ObjectStorage::getByClassAndName( class_name , name ) );
}

Agent* AgentStorage::getByName(QString name) const{
    return dynamic_cast<Agent*>( ObjectStorage::getByName( name ) );
}

QList<Agent*> AgentStorage::getAgentsByClass(QString class_name) const{
    QList<Agent*> agents;
    foreach(QObject* o , ObjectStorage::getByClass( class_name )){
        if( dynamic_cast<Agent*>( o ) ){
            agents.append( dynamic_cast<Agent*>( o ) );
        }
    }
    return agents;
}

QList<Agent*> AgentStorage::getAll() const{
    return this->agents_memory[ Agent::staticMetaObject.className() ].values();
}

bool AgentStorage::contains(Agent *agent) const{
    return this->agents_memory_keys.contains( agent->metaObject()->className() ) && this->agents_memory[ agent->metaObject()->className() ].keys().contains( agent->getId() );
}


/**********************************************************************
 SETTERS
**********************************************************************/

void AgentStorage::add( Agent* agent ){

    // Create storages
    const QMetaObject* class_type = agent->metaObject();
    QList<QString> keys = this->objects.keys();
    while( class_type && !keys.contains( class_type->className() ) ){
        QList< QObject* > list;
        this->objects.insert( class_type->className() , list );

        QHash<QString , QObject* > map1;
        this->object_names.insert( class_type->className() , map1 );

        QHash<unsigned int , Agent*> map2;
        this->agents_memory.insert( class_type->className() , map2 );

        this->agents_memory_keys.append( class_type->className() );

        class_type = class_type->superClass();
    }

    this->mutex.lock();

    // Add to storage
    class_type = agent->metaObject();
    while( class_type ){
        this->objects[ class_type->className() ].append( agent );
        this->object_names[ class_type->className() ].insert( agent->objectName() , agent );
        this->agents_memory[ class_type->className() ].insert( agent->getId() , agent );
        class_type = class_type->superClass();
    }

    this->mutex.unlock();

}

void AgentStorage::remove(Agent* agent){

    // Remove from storage
    const QMetaObject* class_type = agent->metaObject();
    while( class_type ){
        this->objects[ class_type->className() ].removeAll( agent );
        this->object_names[ class_type->className() ].remove( agent->objectName() );
        this->agents_memory[ class_type->className() ].remove( agent->getId() );
        class_type = class_type->superClass();
    }
}
