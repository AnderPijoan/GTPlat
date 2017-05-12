#include "GSSEnvelope.h"

#include <QPainter>
#include <QPen>
#include <QJsonArray>

#include "utils/conversors/image_coordinates_conversor/ImageCoordinatesConversor.h"

GSSEnvelope::GSSEnvelope(QObject *parent) : QObject(parent){
}

GSSEnvelope::GSSEnvelope(double minX, double maxX, double minY, double maxY, QObject *parent) : QObject( parent ){
    this->inner_envelope = geos::geom::Envelope( minX , maxX , minY , maxY );
}

GSSEnvelope::GSSEnvelope(GSSCoordinate c1, GSSCoordinate c2, QObject *parent) : QObject( parent ){
    this->inner_envelope = geos::geom::Envelope( geos::geom::Coordinate( c1.getX() , c1.getY() , c1.getZ() ) , geos::geom::Coordinate( c2.getX() , c2.getY() , c2.getZ() ) );
}

GSSEnvelope::GSSEnvelope(const GSSEnvelope &other) : QObject(){
    this->inner_envelope = geos::geom::Envelope( other.inner_envelope );
}

/**********************************************************************
 EXPORTERS
**********************************************************************/

QJsonObject GSSEnvelope::toGeoJSON(QJsonObject properties) const{

}

QImage GSSEnvelope::toImage(const GSSEnvelope image_bounds, int image_width, int image_height, QColor border_color, int border_width, QColor fill_color) const{
    Q_UNUSED( border_width )

    // Image to be retured
    QImage image = QImage( image_width , image_height , QImage::Format_ARGB32 );
    image.fill( QColor( 0 , 0 , 0 , 0 ) );

    // Painter
    QPainter painter( &image );
    painter.setRenderHint( QPainter::Antialiasing );
    painter.setPen( border_color );

    QPolygon draw;

    // TOP LEFT
    draw.append( ImageCoordinatesConversor::reprojectPoint(this->getMaxY(),this->getMinX(),image_bounds.getMaxY(),image_bounds.getMinY(),image_bounds.getMaxX(),image_bounds.getMinX(),image_width,image_height ) );

    // TOP RIGHT
    draw.append( ImageCoordinatesConversor::reprojectPoint(this->getMaxY(),this->getMaxX(),image_bounds.getMaxY(),image_bounds.getMinY(),image_bounds.getMaxX(),image_bounds.getMinX(),image_width,image_height ) );

    // BOTTOM RIGHT
    draw.append( ImageCoordinatesConversor::reprojectPoint(this->getMinY(),this->getMaxX(),image_bounds.getMaxY(),image_bounds.getMinY(),image_bounds.getMaxX(),image_bounds.getMinX(),image_width,image_height ) );

    // BOTTOM LEFT
    draw.append( ImageCoordinatesConversor::reprojectPoint(this->getMinY(),this->getMinX(),image_bounds.getMaxY(),image_bounds.getMinY(),image_bounds.getMaxX(),image_bounds.getMinX(),image_width,image_height ) );

    // TOP LEFT
    draw.append( ImageCoordinatesConversor::reprojectPoint(this->getMaxY(),this->getMinX(),image_bounds.getMaxY(),image_bounds.getMinY(),image_bounds.getMaxX(),image_bounds.getMinX(),image_width,image_height ) );

    // Draw borders
    painter.drawPolygon( draw );

    // Draw interior
    QPainterPath path;
    path.addPolygon( draw );
    painter.fillPath( path , QBrush( fill_color ) );

    return image;
}

QString GSSEnvelope::toString() const{
    return QString::fromStdString( this->inner_envelope.toString() );
}

/**********************************************************************
 GETTERS
**********************************************************************/

double GSSEnvelope::getMaxX() const{
    return this->inner_envelope.getMaxX();
}

double GSSEnvelope::getMinX() const{
    return this->inner_envelope.getMinX();
}

double GSSEnvelope::getMaxY() const{
    return this->inner_envelope.getMaxY();
}

double GSSEnvelope::getMinY() const{
    return this->inner_envelope.getMinY();
}

AreaUnit GSSEnvelope::getArea() const{
    return AreaUnit( this->inner_envelope.getArea() );
}

GSSCoordinate GSSEnvelope::getCentroid() const{
    geos::geom::Coordinate c;
    this->inner_envelope.centre( c );
    return GSSCoordinate( c.x , c.y , c.z );
}

bool GSSEnvelope::covers(GSSEnvelope env) const{
    return this->inner_envelope.covers( geos::geom::Envelope( env.getMinX() , env.getMaxX() , env.getMinY() , env.getMaxY() ) );
}

bool GSSEnvelope::covers(GSSCoordinate coor) const{
    return this->inner_envelope.covers( geos::geom::Coordinate( coor.getX() , coor.getY() , coor.getZ() ) );
}

bool GSSEnvelope::contains(GSSCoordinate coor) const{
    return this->inner_envelope.contains( geos::geom::Coordinate( coor.getX() , coor.getY() , coor.getZ() ) );
}

bool GSSEnvelope::intersects(GSSEnvelope env) const{
    return this->inner_envelope.intersects( env.inner_envelope );
}

/**********************************************************************
 METHODS
**********************************************************************/

GSSEnvelope GSSEnvelope::createBuffer(double buffer) const{
    geos::geom::Envelope copy = geos::geom::Envelope( this->inner_envelope );
    copy.expandBy( buffer , buffer );
    return GSSEnvelope( copy.getMinX() , copy.getMaxX() , copy.getMinY() , copy.getMaxY() );
}

GSSEnvelope GSSEnvelope::createUnion(GSSEnvelope other) const{
    geos::geom::Envelope copy = geos::geom::Envelope( this->inner_envelope );
    copy.expandToInclude( other.getMaxX() );
    copy.expandToInclude( other.getMaxY() );
    copy.expandToInclude( other.getMinX() );
    copy.expandToInclude( other.getMinY() );
    return GSSEnvelope( copy.getMinX() , copy.getMaxX() , copy.getMinY() , copy.getMaxY() );
}

/**********************************************************************
 OPERATORS
**********************************************************************/

GSSEnvelope& GSSEnvelope::operator= (const GSSEnvelope& other){
    this->inner_envelope = geos::geom::Envelope( other.inner_envelope );
    return *this;
}

bool GSSEnvelope::operator == (const GSSEnvelope& other) const{
    return this->inner_envelope.equals( &other.inner_envelope );
}

bool GSSEnvelope::operator != (const GSSEnvelope& other) const{
    return !(*this == other);
}


