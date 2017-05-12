#include "GSSGeometry.h"

#include "geos/geom/Point.h"
#include "geos/geom/LineString.h"
#include "geos/geom/Polygon.h"

#include <QPainter>
#include <QPen>
#include <QJsonArray>

#include "utils/exporters/debugger/Debugger.h"
#include "utils/geometry/GSSPoint.h"
#include "utils/geometry/GSSLineString.h"
#include "utils/geometry/GSSPolygon.h"

#include "utils/conversors/image_coordinates_conversor/ImageCoordinatesConversor.h"

GSSGeometry::GSSGeometry(geos::geom::Geometry *geom, QObject *parent) :  QObject(parent) , inner_geometry( geom ){
}

GSSGeometry::~GSSGeometry(){
    if( this->inner_geometry ){
        delete this->inner_geometry;
        this->inner_geometry = 0;
    }
}

/**********************************************************************
 EXPORTERS
**********************************************************************/

QImage GSSGeometry::toImage(const GSSEnvelope image_bounds, int image_width, int image_height, QColor border_color, int border_width, QColor fill_color, QImage icon) const{
    Q_UNUSED(fill_color);
    Q_UNUSED(icon);

    // Image to be returned
    QImage image = QImage( image_width , image_height , QImage::Format_ARGB32 );
    image.fill( QColor( 0 , 0 , 0 , 0 ) );

    if( !this->inner_geometry ){ return image; }

    // Painter
    QPainter painter( &image );
    painter.setRenderHint( QPainter::Antialiasing );

    // Just draw representative coordinate
    QPoint draw = ImageCoordinatesConversor::reprojectPoint(
                this->getRepresentativeCoordinate().getY(),
                this->getRepresentativeCoordinate().getX(),
                image_bounds.getMaxY(),
                image_bounds.getMinY(),
                image_bounds.getMaxX(),
                image_bounds.getMinX(),
                image_width,
                image_height );

    QPen pen = QPen( border_color );
    pen.setWidth( image_width / 50 );
    painter.setPen( pen );
    painter.drawPoint( draw );

    return image;
}

QJsonObject GSSGeometry::toGeoJSON( QJsonObject properties ) const{

    // Geometry
    QJsonObject json_geometry;
    json_geometry.insert( "properties" , properties );

    if( !this->inner_geometry ){ return json_geometry; }

    try {

        // POINT
        if( dynamic_cast<const GSSPoint*>(this) ){
            return dynamic_cast<const GSSPoint*>( this )->toGeoJSON( properties );
        }

        // MULTIGEOM
        if( this->inner_geometry->getGeometryTypeId() == geos::geom::GEOS_MULTIPOINT ||
                   this->inner_geometry->getGeometryTypeId() == geos::geom::GEOS_MULTILINESTRING ||
                   this->inner_geometry->getGeometryTypeId() == geos::geom::GEOS_MULTIPOLYGON ) {

            if( this->inner_geometry->getGeometryTypeId() == geos::geom::GEOS_MULTIPOINT ){ json_geometry.insert("type", QString("MultiPoint") ); }
            if( this->inner_geometry->getGeometryTypeId() == geos::geom::GEOS_MULTILINESTRING ){ json_geometry.insert("type", QString("MultiLineString") ); }
            if( this->inner_geometry->getGeometryTypeId() == geos::geom::GEOS_MULTIPOLYGON ){ json_geometry.insert("type", QString("MultiPolygon") ); }

            QJsonArray geomCoordinates;

            for(unsigned int i = 0; i < this->inner_geometry->getNumGeometries(); i++){
                GSSGeometry* g = new GSSGeometry( this->inner_geometry->getGeometryN(i)->clone() );
                geomCoordinates.append( g->toGeoJSON().value("coordinates") );
                delete g;
            }
            json_geometry.insert("coordinates", geomCoordinates);

            // GEOMETRYCOLLECTION
        } else if( this->inner_geometry->getGeometryTypeId() == geos::geom::GEOS_GEOMETRYCOLLECTION ) {

            json_geometry.insert("type" , QString("GeometryCollection") );
            QJsonArray geometries;

            for(unsigned int i = 0; i < this->inner_geometry->getNumGeometries(); i++){
                GSSGeometry* g = new GSSGeometry( this->inner_geometry->getGeometryN( i )->clone() );
                geometries.append( g->toGeoJSON() );
                delete g;
            }
            json_geometry.insert( "geometries" , geometries);

        } else {
            Debugger::warning( this , Q_FUNC_INFO , QString("Geometry type %1 unknown").arg( QString::fromStdString( this->inner_geometry->getGeometryType() ) ) );
        }

    } catch ( ... ){
        Debugger::warning( this , Q_FUNC_INFO , QString("Geometry type %1 parsing crashed").arg( QString::fromStdString( this->inner_geometry->getGeometryType() ) ) );
    }

    return json_geometry;
}

QString GSSGeometry::toString() const{
    return QString::fromStdString( this->inner_geometry->toString() );
}

/**********************************************************************
 GETTERS
**********************************************************************/

GSSCoordinate GSSGeometry::getRepresentativeCoordinate() const{
    return GSSCoordinate( this->inner_geometry->getCoordinate()->x , this->inner_geometry->getCoordinate()->y , this->inner_geometry->getCoordinate()->z );
}

GSSEnvelope GSSGeometry::getEnvelope() const{
    return GSSEnvelope( this->inner_geometry->getEnvelopeInternal()->getMinX() , this->inner_geometry->getEnvelopeInternal()->getMaxX() , this->inner_geometry->getEnvelopeInternal()->getMinY() , this->inner_geometry->getEnvelopeInternal()->getMaxY() );
}

QList<GSSCoordinate> GSSGeometry::getCoordinates() const{
    QList<GSSCoordinate> coors;
    for( unsigned int i = 0; i < this->inner_geometry->getCoordinates()->size() ; i++ ){
        coors.append( GSSCoordinate( this->inner_geometry->getCoordinates()->getAt( i ).x , this->inner_geometry->getCoordinates()->getAt( i ).y , this->inner_geometry->getCoordinates()->getAt( i ).z ) );
    }
    return coors;
}

GSSCoordinate GSSGeometry::getFirstCoordinate() const{
    geos::geom::Coordinate c = this->inner_geometry->getCoordinates()->getAt( 0 );
    return GSSCoordinate( c.x , c.y , c.z );
}

GSSCoordinate GSSGeometry::getLastCoordinate() const{
    geos::geom::Coordinate c = this->inner_geometry->getCoordinates()->getAt( this->inner_geometry->getCoordinates()->size()-1 );
    return GSSCoordinate( c.x , c.y , c.z );
}

bool GSSGeometry::isValid() const{
    return this->inner_geometry ? this->inner_geometry->isValid() : false;
}

bool GSSGeometry::intersects( const GSSGeometry *other ) const{
    return this->inner_geometry->intersects( other->inner_geometry );
}

bool GSSGeometry::equals( const GSSGeometry *other) const{
    return this->inner_geometry->equals( other->inner_geometry );
}

AreaUnit GSSGeometry::getArea() const{
    return AreaUnit( this->inner_geometry ? this->inner_geometry->getArea() : -1 );
}

double GSSGeometry::getDistance( const GSSGeometry *other) const{
    return this->inner_geometry ? this->inner_geometry->distance( other->inner_geometry ) : -1;
}

/**********************************************************************
 METHODS
**********************************************************************/

GSSGeometry* GSSGeometry::createCopy() const{
    return this->castToGeometryType( this->inner_geometry->clone() );
}

GSSPoint* GSSGeometry::createCentroid() const{
    return new GSSPoint( this->inner_geometry->getCentroid() );
}

GSSEnvelope GSSGeometry::createEnvelope() const{
    const geos::geom::Envelope* e = this->inner_geometry->getEnvelopeInternal();
    return GSSEnvelope( e->getMinX() ,  e->getMaxX() ,  e->getMinY() ,  e->getMaxY() );
}

GSSGeometry* GSSGeometry::createUnion(GSSGeometry *other) const{
    return this->castToGeometryType( this->inner_geometry->Union( other->inner_geometry ) );
}

GSSGeometry* GSSGeometry::createIntersection(const GSSGeometry *other) const{
    return this->castToGeometryType( this->inner_geometry->intersection( other->inner_geometry ) );
}


/**********************************************************************
 PRIVATE
**********************************************************************/

GSSGeometry* GSSGeometry::castToGeometryType( geos::geom::Geometry *geom ) const{
    if( geom->getGeometryTypeId() == geos::geom::GEOS_POINT ){
        return new GSSPoint( dynamic_cast<geos::geom::Point*>(geom) );
    }
    if( geom->getGeometryTypeId() == geos::geom::GEOS_LINESTRING ){
        return new GSSLineString( dynamic_cast<geos::geom::LineString*>(geom) );
    }
    if( geom->getGeometryTypeId() == geos::geom::GEOS_POLYGON ){
        return new GSSPolygon( dynamic_cast<geos::geom::Polygon*>(geom) );
    }
    return new GSSGeometry( geom );
}
