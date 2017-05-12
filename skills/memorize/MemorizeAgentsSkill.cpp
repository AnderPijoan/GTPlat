#include "MemorizeAgentsSkill.h"

MemorizeAgentsSkill::MemorizeAgentsSkill(Agent *skilled_agent) : MemorizeSkill(skilled_agent){
    this->storage->deleteLater(); // Delete ObjectStorage
    this->storage = new AgentStorage( skilled_agent ); // Create as AgentStorage
}

/**********************************************************************
 GETTERS
**********************************************************************/

Agent* MemorizeAgentsSkill::getByClassAndId(QString class_name, unsigned int id){
    return dynamic_cast<AgentStorage*>( this->storage )->getByClassAndId( class_name , id );
}

Agent* MemorizeAgentsSkill::getById(unsigned int id){
    return dynamic_cast<AgentStorage*>( this->storage )->getById( id );
}

QList<Agent*> MemorizeAgentsSkill::getAgentsByClass(QString class_name){
    return dynamic_cast<AgentStorage*>( this->storage )->getAgentsByClass( class_name );
}

/**********************************************************************
 SETTERS
**********************************************************************/

void MemorizeAgentsSkill::add(Agent *agent){
    this->storage->add( agent );
}


void MemorizeAgentsSkill::remove(Agent *agent){
    this->storage->remove( agent );
}
