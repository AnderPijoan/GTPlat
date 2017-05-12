#include "ChargingStation.h"

ChargingStation::ChargingStation(GSSGeometry *geom, QObject *parent) : CivilStructure( geom , parent ) , TransportNode( geom->getRepresentativeCoordinate() ){

    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/226/226637.png";

}

