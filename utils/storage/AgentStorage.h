#ifndef AGENTSTORAGE_H
#define AGENTSTORAGE_H

#include "agents/Agent.h"
#include "utils/storage/ObjectStorage.h"
#include <QHash>

class AgentStorage : public ObjectStorage
{
    Q_OBJECT

public:
    AgentStorage(QObject *parent = 0);

    // GETTERS
    Agent* getByClassAndId(QString class_name , unsigned int id) const;
    Agent* getById(unsigned int id) const;
    virtual Agent* getByClassAndName(QString class_name , QString name) const;
    virtual Agent* getByName(QString name) const;
    virtual bool contains(Agent* agent) const;
    QList<Agent*> getAgentsByClass(QString class_name) const;
    QList<Agent*> getAll() const;

    // SETTERS
    virtual void add( Agent* agent );
    virtual void remove( Agent* agent );

protected:

    /**
     * MEMORY
    **/
    QHash<QString, QHash<unsigned int, Agent*> > agents_memory;  // QMAP<ClassName, QMAP<ID , AGENT>>
    QStringList agents_memory_keys; // For faster contains query
};

#endif // AGENTSTORAGE_H
