#ifndef AGENTTOSHP_H
#define AGENTTOSHP_H

#include <QObject>

#include "shapefil.h"
#include "agents/Agent.h"
#include "utils/geometry/GSSGeometry.h"

class AgentToSHP : public QObject
{
    Q_OBJECT
public:

    explicit AgentToSHP(QObject *parent = 0);
    ~AgentToSHP();

    bool exportAgents(QList<Agent*> agents, QString filename, QString geometry_type = "Polygon");

private:
    SHPHandle shp_handle;
    DBFHandle dbf_handle;

    static QString shp_folder_name;
};

#endif // AGENTTOSHP_H
