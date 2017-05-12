#ifndef SHAREDOBJECTSTORAGE_H
#define SHAREDOBJECTSTORAGE_H

#include <QObject>

#include "utils/storage/ObjectStorage.h"

class SharedObjectStorage : public QObject
{
    Q_OBJECT

public:

    static SharedObjectStorage* getInstance();

    // GETTERS
    QObject* getByName(QString name);
    QObject* getByClassAndName(QString class_name , QString name);
    const QList<QObject*> getByClass(QString class_name);

    // SETTERS
    void add( QObject* object );

private:

    SharedObjectStorage();
    SharedObjectStorage(SharedObjectStorage const&);
    ~SharedObjectStorage();

    ObjectStorage* storage;

};

#endif // SHAREDOBJECTSTORAGE_H
