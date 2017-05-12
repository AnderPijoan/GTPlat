#ifndef PLACE_H
#define PLACE_H

#include <QObject>

#include "agents/Agent.h"


class Place : public Agent
{
    Q_OBJECT

public:
    Place( GSSGeometry* geom , QObject* parent = 0);
    virtual QJsonObject toJSON();

    // GETTERS
    AreaUnit getArea();

    // SETTERS
    void setArea( AreaUnit area);

private:
    AreaUnit area; // m2

};

#endif // PLACE_H
