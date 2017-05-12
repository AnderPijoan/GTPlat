#include "ObjectStorage.h"
#include <QDebug>

ObjectStorage::ObjectStorage(QObject *parent) : QObject(parent){

}

/**********************************************************************
 GETTERS
**********************************************************************/

const QStringList ObjectStorage::getClasses() const{
    return this->objects.keys();
}

QObject* ObjectStorage::getByClassAndName(QString class_name, QString name) const{
    if ( this->object_names.keys().contains(class_name) ){
         return this->object_names[class_name].value( name , 0 );
    }
    return 0;
}

QObject* ObjectStorage::getByName(QString name) const{
    foreach(QString class_name , this->object_names.keys()){
        if( this->object_names[ class_name ].contains( name ) ){
            return this->object_names[ class_name ].value( name , 0 );
        }
    }
    return 0;
}

QList< QObject* > ObjectStorage::getByClass(QString class_name) const{
    if ( this->objects.keys().contains( class_name ) ){
         return this->objects.value( class_name );
    }
    QList< QObject* > result;
    return result;
}

bool ObjectStorage::contains(QString class_name) const{
    return this->objects.keys().contains( class_name );
}

bool ObjectStorage::contains(QObject* object) const{
    if( !this->objects.keys().contains( object->metaObject()->className() ) ){
        return false;
    }
    return this->objects.value( object->metaObject()->className() ).contains( object );
}

int ObjectStorage::countObjects(){
    return this->objects.value( QObject::staticMetaObject.className() ).size();
}

/**********************************************************************
 SETTERS
**********************************************************************/

void ObjectStorage::add( QObject* object ){

    // Create storages
    const QMetaObject* class_type = object->metaObject();
    QList<QString> keys = this->objects.keys();
    while( class_type && !keys.contains( class_type->className() ) ){
        QList< QObject* > list;
        this->objects.insert( class_type->className() , list );

        QHash<QString , QObject* > map;
        this->object_names.insert( class_type->className() , map );

        class_type = class_type->superClass();
    }

    this->mutex.lock();

    // Add to storage
    class_type = object->metaObject();
    while( class_type ){
        this->objects[ class_type->className() ].append( object );
        this->object_names[ class_type->className() ].insert( object->objectName() , object );
        class_type = class_type->superClass();
    }

    this->mutex.unlock();
}

void ObjectStorage::remove( QObject* object ){

    this->mutex.lock();

    // Remove from storage
    const QMetaObject* class_type = object->metaObject();
    while( class_type ){
        this->objects[ class_type->className() ].removeAll( object );
        this->object_names[ class_type->className() ].remove( object->objectName() );
        class_type = class_type->superClass();
    }

    this->mutex.unlock();
}

