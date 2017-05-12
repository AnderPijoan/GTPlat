#include "RailwayStop.h"

RailwayStop::RailwayStop(GSSGeometry *geom, QObject *parent) : PublicTransportStop( geom , parent ) {
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/8/8185.png";
    this->getStyle()->color = this->getStyle()->border_color = QColor("Brown");
}

