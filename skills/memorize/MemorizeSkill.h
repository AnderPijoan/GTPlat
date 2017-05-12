#ifndef MEMORIZESKILL_H
#define MEMORIZESKILL_H

#include <QStringList>

#include "skills/Skill.h"
#include "utils/storage/ObjectStorage.h"

class MemorizeSkill : public Skill
{
    Q_OBJECT

public:
    MemorizeSkill(Agent* skilled_agent = 0);
    ~MemorizeSkill();

    // GETTERS
    const QStringList getClasses() const;
    QObject* getByClassAndName(QString class_name , QString name) const;
    QObject* getByName(QString name) const;
    const QList<QObject*> getByClass(QString class_name) const;

    // SETTERS
    virtual void add(QObject* object);
    virtual void remove(QObject* object);

protected:
    ObjectStorage* storage;

};

#endif // MEMORIZESKILL_H
