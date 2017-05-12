#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include "agents/places/Place.h"
#include "skills/store/AgentStoreSkill.h"

#include "geos/geom/Point.h"

class Construction : public Place
{
    Q_OBJECT

public:
    Construction( GSSGeometry* geom , QObject* parent = 0);
    virtual QJsonObject toJSON();

    // GETTERS
    unsigned int getLevels();
    QList<geos::geom::Point*> getEntrances();

    // SETTERS
    void setLevels(int levels);

    // METHODS
    bool enter( Agent* entering_agent );
    bool exit( Agent* exiting_agent );
    QList<Agent*> getAgentsInsideByClass( QString class_name );

private:
    unsigned int levels;
    QList<geos::geom::Point*> entrances;

};

#endif // CONSTRUCTION_H
