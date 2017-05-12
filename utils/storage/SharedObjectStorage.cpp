#include "SharedObjectStorage.h"

#include <QDebug>

SharedObjectStorage* SharedObjectStorage::getInstance(){
    static SharedObjectStorage instance;
    return &instance;
}

SharedObjectStorage::SharedObjectStorage() : QObject(){
    this->storage = new ObjectStorage( this );
}

SharedObjectStorage::~SharedObjectStorage(){
    this->storage->deleteLater();
}

/**********************************************************************
 GETTERS
**********************************************************************/

QObject* SharedObjectStorage::getByClassAndName(QString class_name, QString name){
    return this->storage->getByClassAndName( class_name , name );
}


QObject* SharedObjectStorage::getByName(QString name){
    return this->storage->getByName( name );
}

const QList<QObject*> SharedObjectStorage::getByClass(QString class_name){
    return this->storage->getByClass( class_name );
}

/**********************************************************************
 SETTERS
**********************************************************************/

void SharedObjectStorage::add(QObject *object ){
    this->storage->add( object );
}


