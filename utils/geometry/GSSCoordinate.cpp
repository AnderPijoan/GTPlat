#include "GSSCoordinate.h"

GSSCoordinate::GSSCoordinate(QObject *parent) : QObject(parent){
}

GSSCoordinate::GSSCoordinate(double x, double y, double z , QObject *parent ) : QObject( parent ){
    this->inner_coordinate = geos::geom::Coordinate( x , y , z );
}

GSSCoordinate::GSSCoordinate(const GSSCoordinate &other) : QObject(){
    this->inner_coordinate = geos::geom::Coordinate( other.inner_coordinate );
}

GSSCoordinate::~GSSCoordinate(){
    this->inner_coordinate.setNull();
}

/**********************************************************************
 EXPORTERS
**********************************************************************/

QString GSSCoordinate::toString() const{
    return QString("(%1,%2,%3)")
            .arg( this->getX() )
            .arg( this->getY() )
            .arg( this->getZ() );
}

/**********************************************************************
 GETTERS
**********************************************************************/

bool GSSCoordinate::isValid() const{
    return !this->inner_coordinate.isNull();
}

double GSSCoordinate::getX() const{
    return this->inner_coordinate.isNull() ? 0 : this->inner_coordinate.x;
}

double GSSCoordinate::getY() const{
    return this->inner_coordinate.isNull() ? 0 : this->inner_coordinate.y;
}

double GSSCoordinate::getZ() const{
    return this->inner_coordinate.isNull() ? 0 : this->inner_coordinate.z;
}

double GSSCoordinate::distance(GSSCoordinate other) const{
    return this->inner_coordinate.distance( other.inner_coordinate );
}

/**********************************************************************
 OPERATORS
**********************************************************************/
bool GSSCoordinate::operator == (const GSSCoordinate& other) const{
    return this->inner_coordinate.equals3D( other.inner_coordinate );
}

bool GSSCoordinate::operator != (const GSSCoordinate& other) const{
    return !(*this == other);
}

bool GSSCoordinate::operator < (const GSSCoordinate& other) const{
    return this->inner_coordinate < other.inner_coordinate;
}

GSSCoordinate& GSSCoordinate::operator = (const GSSCoordinate& other){
    this->inner_coordinate = geos::geom::Coordinate( other.inner_coordinate );
    return *this;
}
