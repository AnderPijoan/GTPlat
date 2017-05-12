#include "AgentStoreSkill.h"

AgentStoreSkill::AgentStoreSkill(Agent* agent) : Skill(agent){
    this->max_capacity = 1;
    this->storage = new AgentStorage( this );
}

AgentStoreSkill::~AgentStoreSkill(){
    this->storage->deleteLater();
}

/**********************************************************************
 GETTERS
**********************************************************************/

bool AgentStoreSkill::isAgentInside( Agent* agent ) const{
    return this->storage->contains( agent );
}

bool AgentStoreSkill::isFull() const{
    return this->storage->countObjects() >= int(this->max_capacity);
}

bool AgentStoreSkill::isEmpty() const{
    return this->storage->countObjects() <= 0;
}

unsigned int AgentStoreSkill::getMaxCapacity() const{
    return this->max_capacity;
}

unsigned int AgentStoreSkill::countAgentsInside() const{
    return this->storage->countObjects();
}

const QList<Agent*> AgentStoreSkill::getAgentsInsideByClass(QString class_name) const{
    return this->storage->getAgentsByClass( class_name );
}

const QList<Agent*> AgentStoreSkill::getAgentsInside() const{
    return this->storage->getAll();
}

/**********************************************************************
 SETTERS
**********************************************************************/

void AgentStoreSkill::setMaxCapacity(unsigned int capacity) {
    this->mutex.lock();
    this->max_capacity = capacity;
    this->mutex.unlock();
}

/**********************************************************************
 METHODS
**********************************************************************/

bool AgentStoreSkill::enter(Agent* agent){

    // See if it is already inside
    if ( this->storage->contains( agent ) ){
        return true;
    }

    this->mutex.lock();
    if ( !this->isFull() ) {
        this->storage->add(agent);
        this->mutex.unlock();
        return true;
    }

    this->mutex.unlock();
    return false;
}

bool AgentStoreSkill::exit(Agent* agent){
    this->mutex.lock();
    if( !this->storage->contains( agent ) ){
        this->mutex.unlock();
        return true;
    }
    this->storage->remove( agent ); // Returns amount of times removed the agent
    this->mutex.unlock();
    return true;
}

void AgentStoreSkill::clear(){
    this->mutex.lock();
    foreach(Agent* agent , this->storage->getAll()){
        this->storage->remove( agent );
    }
    this->mutex.unlock();
}
