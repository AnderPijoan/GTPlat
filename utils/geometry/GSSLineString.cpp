#include "GSSLineString.h"

#include <QPainter>
#include <QPen>
#include <QJsonArray>

#include "utils/conversors/image_coordinates_conversor/ImageCoordinatesConversor.h"

GSSLineString::GSSLineString( geos::geom::LineString *geom, QObject *parent) : GSSGeometry( geom , parent ){

}

/**********************************************************************
 EXPORTERS
**********************************************************************/

QImage GSSLineString::toImage(const GSSEnvelope image_bounds, int image_width, int image_height, QColor border_color, int border_width, QColor fill_color, QImage icon) const{
    Q_UNUSED(fill_color);
    Q_UNUSED(icon);

    // If bounds are too big, just return coordinate
    if( image_bounds.getArea() > (this->createEnvelope().getArea() * 100) ){
        return GSSGeometry::toImage( image_bounds , image_width , image_height , fill_color , border_width , fill_color , icon );
    }

    // Image to be returned
    QImage image = QImage( image_width , image_height , QImage::Format_ARGB32 );
    image.fill( QColor( 0 , 0 , 0 , 0 ) );

    if( !this->inner_geometry ){ return image; }

    // Painter
    QPainter painter( &image );
    painter.setRenderHint( QPainter::Antialiasing );
    QPen pen( border_color );
    pen.setWidth( image_width / 50 );
    painter.setPen( pen );

    const geos::geom::LineString* line = dynamic_cast<const geos::geom::LineString*>( this->inner_geometry );

    // If bounds are too big, skip some coordinates
    unsigned int incr = 1;
    if( image_bounds.getArea().number() > this->getLength().number() * 5 ){ incr = 2; }
    if( image_bounds.getArea().number() > this->getLength().number() * 50 ){ incr = 5; }

    for( unsigned int i = 0; i < line->getCoordinates()->size()-1; i+=incr){

        QLine draw;
        if( !line->getCoordinateN(i).isNull() && !line->getCoordinateN(i+1).isNull() ){
            draw = QLine(
                        ImageCoordinatesConversor::reprojectPoint(
                            line->getCoordinateN(i).y,
                            line->getCoordinateN(i).x,
                            image_bounds.getMaxY(),
                            image_bounds.getMinY(),
                            image_bounds.getMaxX(),
                            image_bounds.getMinX(),
                            image_width,
                            image_height ) ,
                        ImageCoordinatesConversor::reprojectPoint(
                            line->getCoordinateN(i+1).y,
                            line->getCoordinateN(i+1).x,
                            image_bounds.getMaxY(),
                            image_bounds.getMinY(),
                            image_bounds.getMaxX(),
                            image_bounds.getMinX(),
                            image_width,
                            image_height ) );
        }

        // Draw line
        painter.drawLine( draw );
    }

    return image;
}

QJsonObject GSSLineString::toGeoJSON(QJsonObject properties) const{

    // Geometry
    QJsonObject json_geometry;
    json_geometry.insert( "properties" , properties );

    if( !this->inner_geometry ){ return json_geometry; }

    if( this->inner_geometry->getGeometryTypeId() == geos::geom::GEOS_LINESTRING ){

        geos::geom::LineString* line = dynamic_cast<geos::geom::LineString*>( this->inner_geometry->clone() );

        json_geometry.insert("type", QString("LineString") );

        QJsonArray lineCoordinates;

        for(unsigned int i = 0; i < line->getCoordinates()->size(); i++){
            if( !line->getCoordinates()->getAt(i).isNull() ){
                QJsonArray coordinatePair;
                coordinatePair.append( line->getCoordinates()->getAt(i).x );
                coordinatePair.append( line->getCoordinates()->getAt(i).y );
                coordinatePair.append( line->getCoordinates()->getAt(i).z );
                lineCoordinates.append(coordinatePair);
            }
        }
        json_geometry.insert("coordinates", lineCoordinates);

        delete line;
        return json_geometry;
    }

    return GSSGeometry::toGeoJSON( properties );
}

/**********************************************************************
 GETTERS
**********************************************************************/

LengthUnit GSSLineString::getLength() const{
    return LengthUnit( dynamic_cast<const geos::geom::LineString*>( this->inner_geometry )->getLength() );
}

/**********************************************************************
 METHODS
**********************************************************************/

GSSLineString* GSSLineString::createReversed() const{
    const geos::geom::LineString* l = dynamic_cast<const geos::geom::LineString*>( this->inner_geometry );
    return new GSSLineString( dynamic_cast<geos::geom::LineString*>( l->reverse() ) );
}

