#include "BusStop.h"

BusStop::BusStop(GSSGeometry *geom, QObject *parent) : PublicTransportStop(geom , parent ){
    this->getStyle()->color = this->getStyle()->border_color = QColor("Navy");
}

