#ifndef PERSON_H
#define PERSON_H

#include "agents/Agent.h"

#include "agents/people/PersonConstants.h"

class Person : public Agent
{
    Q_OBJECT
public:

    Person(PersonConstants::gender gender = PersonConstants::MALE , GSSGeometry* geom = 0 , QObject* parent = 0);
    ~Person();
    virtual QJsonObject toJSON();
    virtual void fromJSON(QJsonObject json);

    // GETTERS
    PersonConstants::gender getGender();

    // SETTERS

protected:
    PersonConstants::gender gender;
};

#endif // PERSON_H
