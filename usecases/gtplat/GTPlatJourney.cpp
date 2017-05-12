#include "GTPlatJourney.h"

GTPlatJourney::GTPlatJourney(QObject *parent) : QObject(parent){

}

QJsonObject GTPlatJourney::toJSON(){
    QJsonObject json;
    json.insert( "profile" , this->profile->objectName() );
    json.insert( "amount" , this->amount );
    return json;
}

void GTPlatJourney::fromJSON(QJsonObject json ){
    this->amount = json.value("amount").toInt();
    this->date = QDate::fromString( json.value("date").toString( QDateTime::currentDateTime().date().toString("MM-dd-yyyy") ) , "MM-dd-yyyy" );
    this->time = QTime::fromString( json.value("time").toString( QDateTime::currentDateTime().time().toString("hh:mm:ss") ) , "hh:mm:ss" );
    this->train_with = json.value("train_with").toString();
    this->time_deviation = json.value("time_deviation").toDouble(60.0);
    this->multiply_by = json.value("multiply_by").toDouble(1);
    this->profile = 0;
    this->from = 0;
    this->to = 0;
}
