#include "GTPlatIncentive.h"

GTPlatIncentive::GTPlatIncentive(QObject *parent) : QObject(parent){

    this->active = true;
}

QJsonObject GTPlatIncentive::toJSON(){
    QJsonObject json;
    json.insert( "name" , this->name );
    return json;
}

void GTPlatIncentive::fromJSON(QJsonObject json){
    this->name = json.value("name").toString("NO_NAME");
    this->active = json.value("active").toBool(true);

    this->public_transport_price_mult = json.value("public_transport_price_mult").toDouble(1);
    this->public_transport_price_add = json.value("public_transport_price_add").toDouble(0);

    this->public_transport_time_mult = json.value("public_transport_time_mult").toDouble(1);
    this->public_transport_time_add = json.value("public_transport_time_add").toDouble(0);

    this->transfer_time_mult = json.value("transfer_time_mult").toDouble(1);
    this->transfer_time_add = json.value("transfer_time_add").toDouble(0);

    this->private_vehicle_time_mult = json.value("private_vehicle_time_mult").toDouble(1);
    this->private_vehicle_time_add = json.value("private_vehicle_time_add").toDouble(0);

    this->private_vehicle_price_mult = json.value("private_vehicle_price_mult").toDouble(1);
    this->private_vehicle_price_add = json.value("private_vehicle_price_add").toDouble(0);
}

