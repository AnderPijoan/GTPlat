#include "GTPlatProfile.h"

GTPlatProfile::GTPlatProfile(QObject *parent) : QObject(parent){
    this->more_expensive = 0.25;
    this->more_polutant = 0.25;
    this->longer = 0.25;
    this->slower = 0.25;
}

GTPlatProfile::GTPlatProfile( QJsonObject json ){
    this->setObjectName( json.value("name").toString("NO_NAME") );
    this->more_polutant = json.value("greener").toDouble(0.25);
    this->slower = json.value("quicker").toDouble(0.25);
    this->more_expensive = json.value("cheaper").toDouble(0.25);
    this->longer = json.value("shorter").toDouble(0.25);
    QJsonArray options_array = json.value("options").toArray();
    foreach(QJsonValue v , options_array ){
        this->options.append( v.toString("WALK") );
    }
}


QJsonObject GTPlatProfile::toJSON(){
    QJsonObject json;
    json.insert( "name" , this->objectName() );
    return json;
}


