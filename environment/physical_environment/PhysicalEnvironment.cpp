#include <QDebug>
#include <QJsonObject>

#include "PhysicalEnvironment.h"
#include "environment/Environment.h"

PhysicalEnvironment::PhysicalEnvironment(Environment* parent) : QObject(parent){
    this->geometry_controller = new GSSGeometryController( this );
}

PhysicalEnvironment::~PhysicalEnvironment(){
    qDeleteAll(this->spatial_index.values());
    this->geometry_controller->deleteLater();
}

/**********************************************************************
 GETTERS
**********************************************************************/

const GSSGeometryController* PhysicalEnvironment::getGeometryController() const{
    return this->geometry_controller;
}

QList<Agent*> PhysicalEnvironment::orderByDistance(Agent* source, QList<Agent*> agents) const{
    QList<Agent*> ordered;
    if( !agents.size() ){
        return ordered;
    }

     while( agents.size() ){
        Agent* nearest = agents.at( 0 );
        const GSSGeometry* nearest_geom = nearest->getGeometry();

        foreach( Agent* agent , agents ){
            const GSSGeometry* agent_geom = agent->getGeometry();

            if( source->getGeometry()->getDistance( agent_geom ) < source->getGeometry()->getDistance( nearest_geom ) ){
                nearest = agent;
                nearest_geom = agent_geom;
            }
        }

        ordered.append( nearest );
        agents.removeOne( nearest );
    }

    return ordered;
}


QList<Agent*> PhysicalEnvironment::getAgentsInsideEnvelope(const GSSEnvelope envelope, QString class_name) const{

    QList<Agent*> agents;

    if( this->spatial_index.keys().contains(class_name) ){
        foreach( void * o , this->spatial_index[class_name]->getElements( envelope ) ){
            Agent* agent = ((Agent*)o);
            agents.append( agent );
        }
    }

    return agents;
}


QList<Agent*> PhysicalEnvironment::getAgentsIntersecting(const GSSGeometry* geometry, QString class_name) const{
    QList<Agent*> agents;

    // Precisely compare inside geom
    foreach(Agent* agent , this->getAgentsInsideEnvelope( geometry->getEnvelope() , class_name ) ){
        if( geometry->intersects( agent->getGeometry() ) ){
            agents.append( agent );
        }
    }
    return agents;
}

/**
 * @brief PhysicalEnvironment::getNearestAgents Given a list of geometries and the class_name of agents in the
 * environment, returns an ordered list of the nearest agent for each geometry.
 * IMPORTANT, if an agent is not snapped it will return a 0 agent to preserve list size and order.
 * @param geometries List of geometries from where to start searching
 * @param class_name ClassName of the agents in the environment from which to get the nearest for each geometry
 * @return
 */
QList<Agent*> PhysicalEnvironment::getNearestAgents(QList<GSSCoordinate> coors, QString class_name) const{
    QList<Agent*> founds = QList<Agent*>();
    foreach(GSSCoordinate coor , coors){
        founds.append( this->getNearestAgent( coor , class_name ) );
    }
    return founds;
}

Agent* PhysicalEnvironment::getNearestAgent(GSSCoordinate coor, QList<Agent*> agents) const{
    QList<GSSCoordinate> coors;
    coors.append( coor );
    QList<Agent*> nearests = this->getNearestAgents( coors , agents );
    if( !nearests.isEmpty() ){
        return nearests.at(0);
    }
    return 0;
}

/**
 * @brief PhysicalEnvironment::getNearestAgents Given a list of geometries and the class_name of agents in the
 * environment, returns an ordered list of the nearest agent for each geometry.
 * @param geometry Geometry from where to start searching
 * @param class_name ClassName of the agents in the environment from which to get the nearest for geometry
 * @return
 */
Agent* PhysicalEnvironment::getNearestAgent(GSSCoordinate coor, QString class_name) const{
    Agent* found = 0;
    if( this->spatial_index.keys().contains(class_name) ){

        GSSEnvelope env = GSSEnvelope( coor.getX() , coor.getX() , coor.getY() , coor.getY() );
        QList<void *> objs = this->spatial_index.value(class_name)->getElements( env );

        if( !objs.isEmpty() ){
            found = ( (Agent*) objs.at( qrand() % objs.size() ) );
            foreach(void * o , objs){
                Agent* agent = ((Agent*)o);
                if( agent && coor.distance( agent->getRepresentativeCoordinate() ) < coor.distance( found->getRepresentativeCoordinate() )){
                    found = agent;
                }
            }
        }
    }
    return found;
}

/**
 * @brief PhysicalEnvironment::getNearestAgents Given a list of geometries and some agents to search, returns
 * an ordered list of the nearest agent for each geometry.
 * IMPORTANT, if an agent is not snapped it will return a 0 agent to preserve list size and order.
 * @param geometries List of geometries from where to start searching
 * @param agents List of agents from which to get the nearest for each geometry
 * @return
 */
QList<Agent*> PhysicalEnvironment::getNearestAgents(QList<GSSCoordinate> coors, QList<Agent*> agents) const{
    QList<Agent*> founds;

    GSSQuadtree* index = new GSSQuadtree();
    foreach(Agent* agent , agents){
        index->insert( agent->getGeometry()->getEnvelope() , agent );
    }

    foreach(GSSCoordinate coor , coors){
        Agent* found = 0;
        GSSEnvelope env = GSSEnvelope( coor.getX() , coor.getX() , coor.getY() , coor.getY() );
        QList<void *> objs = index->getElements( env );

        if( !objs.isEmpty() ){
            found = ( (Agent*) objs.at( qrand() % objs.size() ) );
            foreach(void * o , objs){
                Agent* agent = ((Agent*) o);
                if( agent && coor.distance( agent->getRepresentativeCoordinate() ) < coor.distance( found->getRepresentativeCoordinate() ) ){
                    found = agent;
                }
            }
        }
        founds.append( found );
    }

    delete index;
    return founds;
}


Agent* PhysicalEnvironment::getAgentByGeometry(GSSGeometry* geometry, QString class_name) const{
    if( this->spatial_index.contains(class_name) ){

        QList<void *> objs = this->spatial_index[class_name]->getElements( geometry->getEnvelope() );

        if( !objs.isEmpty() ){
            foreach(void * o , objs){
                Agent* agent = ( (Agent*) o);
                if( agent->getGeometry()->equals( geometry ) ){
                    return agent;
                }
            }
        }
    }
    return 0;
}

/**********************************************************************
 PRIVATE
**********************************************************************/

bool PhysicalEnvironment::addAgent(Agent *agent){

    if( !agent->geometry ){
        qWarning() << Q_FUNC_INFO << "Tried to add agent without geometry" << agent->metaObject()->className() << agent->getId() << endl;
        return false;
    }

    this->mutex.lock();
    this->spatial_envelopes.insert( agent->getId() , agent->geometry->getEnvelope() );
    this->mutex.unlock();

    const QMetaObject* class_type = agent->metaObject();
    QList<QString> keys = this->spatial_index.keys();
    while( class_type && !keys.contains( class_type->className() ) ){

        // Insert new quadtree with the agents class
        this->mutex.lock();
        GSSQuadtree* index = new GSSQuadtree();
        this->spatial_index.insert( class_type->className() , index );
        this->mutex.unlock();

        class_type = class_type->superClass();
    }

    class_type = agent->metaObject();
    GSSEnvelope env = agent->geometry->getEnvelope();
    while( class_type ){

        this->mutex.lock();
        this->spatial_index[ class_type->className() ]->insert( env , agent );
        this->mutex.unlock();

        class_type = class_type->superClass();
    }

    return true;
}

bool PhysicalEnvironment::setAgentGeometry(Agent *agent, GSSGeometry *new_geom){

    // Remove old version from spatial index
    this->removeAgent( agent );

    // Set geometry
    agent->mutex.lock();
    if( agent->geometry ){
        agent->geometry->deleteLater();
        agent->geometry = 0;
    }
    if( new_geom ){
        agent->geometry = new_geom->createCopy();
        new_geom->deleteLater();
    }
    agent->mutex.unlock();

    // Add new version in spatial index
    this->addAgent( agent );
    return true;
}

bool PhysicalEnvironment::removeAgent(Agent *agent){

    // Retrieve its envelope
    GSSEnvelope agent_geom_env = this->spatial_envelopes.value( agent->getId() );

    QList<QString> keys = this->spatial_index.keys();
    const QMetaObject* obj = agent->metaObject();
    while( obj ){

        // Remove from spatial index
        this->mutex.lock();
        if( keys.contains( obj->className() ) ){
            this->spatial_index.value( obj->className() )->remove( agent_geom_env , agent );
        }
        this->mutex.unlock();

        obj = obj->superClass();
    }

    this->mutex.lock();
    this->spatial_envelopes.remove( agent->getId() );
    this->mutex.unlock();

    return true;
}


