#include "Construction.h"

Construction::Construction(GSSGeometry* geom , QObject *parent) : Place( geom , parent ){
    this->levels = 1;

    AgentStoreSkill* skill = new AgentStoreSkill(this);
    skill->setMaxCapacity( 9999999 );
    this->addSkill( skill );

    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/46/46517.png";
}

QJsonObject Construction::toJSON(){
    QJsonObject agent_json = Place::toJSON();
    agent_json.insert( "height", CustomJsonExporter::format( this->getLevels() * 3 ) );
    agent_json.insert( "levels", CustomJsonExporter::format( this->getLevels() ) );
    return agent_json;
}

/**********************************************************************
 GETTERS
**********************************************************************/

unsigned int Construction::getLevels(){
    return this->levels;
}

QList<geos::geom::Point*> Construction::getEntrances(){
    return this->entrances;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Construction::setLevels(int levels){
    this->levels = levels;
}

/**********************************************************************
 METHODS
**********************************************************************/

bool Construction::enter(Agent *entering_agent){
    AgentStoreSkill* skill = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className() ) );
    return skill->enter( entering_agent );
}

bool Construction::exit(Agent *exiting_agent){
    AgentStoreSkill* skill = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className() ) );
    return skill->exit( exiting_agent );
}

QList<Agent*> Construction::getAgentsInsideByClass(QString class_name){
    AgentStoreSkill* skill = dynamic_cast<AgentStoreSkill*>( this->getSkill( AgentStoreSkill::staticMetaObject.className() ) );
    return skill->getAgentsInsideByClass( class_name );
}
