#include "MemorizeSkill.h"

#include "agents/Agent.h"

MemorizeSkill::MemorizeSkill(Agent* skilled_agent) : Skill(skilled_agent){
    this->storage = new ObjectStorage( skilled_agent );
}

MemorizeSkill::~MemorizeSkill(){
    this->storage->deleteLater();
}

/**********************************************************************
 GETTERS
**********************************************************************/

const QStringList MemorizeSkill::getClasses() const{
    return this->storage->getClasses();
}

QObject* MemorizeSkill::getByClassAndName(QString class_name, QString name) const{
    return this->storage->getByClassAndName( class_name , name );
}

QObject* MemorizeSkill::getByName(QString name) const{
    return this->storage->getByName( name );
}

const QList<QObject*> MemorizeSkill::getByClass(QString class_name) const{
    return this->storage->getByClass( class_name );
}

/**********************************************************************
 SETTERS
**********************************************************************/

void MemorizeSkill::add(QObject *object){
    this->storage->add( object );
}


void MemorizeSkill::remove(QObject *object){
    this->storage->remove( object );
}
