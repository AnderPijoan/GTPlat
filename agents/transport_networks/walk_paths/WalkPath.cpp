#include "WalkPath.h"

#include "agents/people/Person.h"

WalkPath::WalkPath(WalkPathConstants::walk_path_types type, TransportNode* from, TransportNode* to, GSSLineString* geom , QObject* parent) : TransportLine(from , to , geom , parent){
    this->setWalkPathType( type );
    this->setMaxSpeed( WalkPathConstants::defaultSpeed( this->getWalkPathType() ) );
    this->getStyle()->color = this->getStyle()->border_color = QColor("Orange");
    this->getStyle()->zoom_level = 15;
}

QJsonObject WalkPath::toJSON(){
    QJsonObject agent_json = TransportLine::toJSON();
    agent_json.insert( "walk-path-type", CustomJsonExporter::format( WalkPathConstants::toString( this->walk_path_type ) ) );
    return agent_json;
}

void WalkPath::fromJSON(QJsonObject json){
    TransportLine::fromJSON(json);
    if( json.contains( "properties" ) ){
        QJsonObject properties = json["properties"].toObject();
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

WalkPathConstants::walk_path_types WalkPath::getWalkPathType(){
    return this->walk_path_type;
}

double WalkPath::getCost() const{
    return this->getLength().number();
}


/**********************************************************************
 SETTERS
**********************************************************************/

void WalkPath::setWalkPathType(WalkPathConstants::walk_path_types type){
    this->walk_path_type = type;
    this->allowed_agent_classes.append( Person::staticMetaObject.className() );
}
