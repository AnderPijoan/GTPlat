#include "Person.h"

#include "environment/Environment.h"
#include "environment/physical_environment/PhysicalEnvironment.h"
#include "skills/memorize/MemorizeSkill.h"
#include "skills/route/WalkPathRoutingSkill.h"
#include "skills/move/MoveSkill.h"
#include "agents/transport_networks/walk_paths/WalkPath.h"

Person::Person(PersonConstants::gender gender , GSSGeometry* geom , QObject* parent) : Agent( geom , parent ) {
    this->gender = gender;
    this->getStyle()->icon_url = "http://cdn.flaticon.com/png/128/17447.png";

    this->addSkill( new MoveSkill(this) );
    this->addSkill( new WalkPathRoutingSkill( this ) );
    this->addSkill( new MemorizeSkill(this) );
}

Person::~Person(){

}

QJsonObject Person::toJSON(){
    QJsonObject agent_json = Agent::toJSON();
    agent_json.insert( "born-date", CustomJsonExporter::format( this->getBornDatetime() ) );
    agent_json.insert( "gender", CustomJsonExporter::format( this->getGender() ) );
    return agent_json;
}

void Person::fromJSON(QJsonObject json){
    Agent::fromJSON(json);
    if( json.contains( "properties" ) ){
    QJsonObject properties = json["properties"].toObject();
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

PersonConstants::gender Person::getGender(){
    return this->gender;
}

/**********************************************************************
 SETTERS
**********************************************************************/
