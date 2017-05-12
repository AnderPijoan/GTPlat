#ifndef OBJECTSTORAGE_H
#define OBJECTSTORAGE_H

#include <QObject>
#include <QMetaObject>
#include <QHash>
#include <QMutex>
#include <QStringList>
#include <QDebug>

class ObjectStorage : public QObject
{
    Q_OBJECT

public:
    explicit ObjectStorage(QObject *parent = 0);

    // GETTERS
    const QStringList getClasses() const;
    virtual QObject* getByClassAndName(QString class_name , QString name) const;
    virtual QObject* getByName(QString name) const;
    virtual QList< QObject* > getByClass(QString class_name) const;
    virtual bool contains( QObject* object ) const;
    bool contains( QString class_name ) const;
    int countObjects();

    // SETTERS
    virtual void add( QObject* object );
    virtual void remove( QObject* object );

protected:

    /**
     * MEMORY
    **/
    QHash<QString, QList< QObject* > > objects;  // QMAP<ClassName, QList<QOBJECT>>
    QHash<QString, QHash<QString , QObject* > > object_names;  // QHash<ClassName, QHash<NAME, QOBJECT>>

    QMutex mutex;
};

#endif // OBJECTSTORAGE_H
