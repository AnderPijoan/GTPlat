#include "Accommodation.h"

Accommodation::Accommodation( GSSGeometry* geom , QObject *parent) : Construction( geom , parent ){
    this->getStyle()->icon_url = "http://image.flaticon.com/icons/png/128/129/129524.png";
}

