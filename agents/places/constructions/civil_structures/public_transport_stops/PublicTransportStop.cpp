#include "PublicTransportStop.h"

#include "skills/store/AgentStoreSkill.h"

PublicTransportStop::PublicTransportStop( GSSGeometry* geom , QObject* parent ) : CivilStructure( geom , parent ) , TransportNode( geom->getRepresentativeCoordinate() ){
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/224/224490.png";

    AgentStoreSkill* contain_skill = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className() ) );
    contain_skill->setMaxCapacity( 99999 );
}


/**********************************************************************
 GETTERS
**********************************************************************/

QString PublicTransportStop::getZoneId(){
    return this->zone_id;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void PublicTransportStop::setZoneId( QString zone_id ){
    this->zone_id = zone_id;
}

bool PublicTransportStop::addWaitingPerson(Agent *agent){
    AgentStoreSkill* contain_skill = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className() ) );
    return contain_skill->enter( agent );
}

bool PublicTransportStop::removeWaitingPerson(Agent *agent){
    AgentStoreSkill* contain_skill = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className() ) );
    return contain_skill->exit( agent );
}

/**********************************************************************
 METHODS
**********************************************************************/

