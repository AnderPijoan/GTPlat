#include <QJsonDocument>
#include <QJsonObject>

#include "TimeEnvironment.h"
#include "environment/Environment.h"

#include "utils/exporters/custom_json_exporter/CustomJsonExporter.h"

TimeEnvironment::TimeEnvironment(Environment* parent) : QObject(parent) , time_speed(10) {
    this->datetime = QDateTime::currentDateTime();
}

TimeEnvironment::~TimeEnvironment(){
}

QJsonObject TimeEnvironment::toJSON(){
    QJsonObject json;
    json.insert( "date-time" , CustomJsonExporter::format( this->getCurrentDateTime() ) );
    json.insert( "speed" , CustomJsonExporter::format( this->getTimeSpeed() ) );
    return json;
}

void TimeEnvironment::fromJSON(QJsonObject json){
    if( json.contains("speed") ){
        this->setTimeSpeed( qMax( json["speed"].toObject()["value"].toDouble() , 0.1) );
    }
}

/**********************************************************************
   GETTERS
**********************************************************************/

QDateTime TimeEnvironment::getCurrentDateTime() const{
    return this->datetime;
}

double TimeEnvironment::getTimeSpeed() const{
    return this->time_speed;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void TimeEnvironment::setDate(QDate& date){
    if( date.isValid() ){
        this->datetime.setDate( date );
    } else {
        Debugger::warning( this , Q_FUNC_INFO , "Invalid date" );
    }
}

void TimeEnvironment::setTime(QTime& time){
    if( time.isValid() ){
        this->datetime.setTime( time );
    } else {
        Debugger::warning( this , Q_FUNC_INFO , "Invalid time" );
    }
}

void TimeEnvironment::setDatetime(QDateTime datetime){
    if( datetime.isValid() ){
        this->datetime = datetime;
    } else {
        Debugger::warning( this , Q_FUNC_INFO , "Invalid datetime" );
    }
}

void TimeEnvironment::setTimeSpeed(double time_speed){
    this->time_speed = qMax(0.01 , time_speed); // Avoid time_speed = 0
}

