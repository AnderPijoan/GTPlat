#ifndef PHYSICALENVIRONMENT_H
#define PHYSICALENVIRONMENT_H

#include <QObject>
#include <QMutex>

#include "utils/geometry/GSSGeometryController.h"
#include "utils/geometry/GSSQuadtree.h"
#include "geos/index/quadtree/Quadtree.h"

#include "agents/Agent.h"

QT_FORWARD_DECLARE_CLASS(Environment)

using namespace geos::geom;
using namespace geos::index::quadtree;

class PhysicalEnvironment : public QObject
{
    Q_OBJECT
    friend class Environment; // Environment can access this constructor

public:

    // GETTERS
    const GSSGeometryController* getGeometryController() const;

    // SPATIAL GETTERS
    QList<Agent*> orderByDistance( Agent* source , QList<Agent*> agents ) const;
    QList<Agent*> getAgentsInsideEnvelope( const GSSEnvelope envelope, QString class_name ) const;
    QList<Agent*> getAgentsIntersecting( const GSSGeometry* geometry, QString class_name ) const;
    Agent* getNearestAgent( GSSCoordinate coor, QString class_name ) const;
    Agent* getNearestAgent( GSSCoordinate coor, QList<Agent*> agents ) const;
    QList<Agent*> getNearestAgents( QList<GSSCoordinate> coors, QString class_name ) const;
    QList<Agent*> getNearestAgents( QList<GSSCoordinate> coors, QList<Agent*> agents ) const;
    Agent* getAgentByGeometry( GSSGeometry* geometry, QString class_name ) const;

private:
    explicit PhysicalEnvironment(Environment* parent);
    ~PhysicalEnvironment();

    // AGENT METHODS
    bool addAgent( Agent* agent );
    bool setAgentGeometry( Agent* agent , GSSGeometry* new_geom );
    bool removeAgent( Agent* agent );

    GSSGeometryController* geometry_controller; // In charge of all geometries

    // SPATIAL INDEX
    QMap<unsigned int , GSSEnvelope> spatial_envelopes; // QMAP< AGENT_ID , ENVELOPE> Spatial envelopes
    QMap<QString , GSSQuadtree*> spatial_index; // Spatial indexes

    // Mutex, for avoiding concurrency
    QMutex mutex;
};

#endif // PHYSICALENVIRONMENT_H
