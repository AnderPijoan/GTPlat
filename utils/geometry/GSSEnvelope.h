#ifndef GSSENVELOPE_H
#define GSSENVELOPE_H

#include <QObject>
#include <QJsonObject>
#include <QImage>
#include <QColor>
#include "utils/geometry/GSSCoordinate.h"

#include "utils/units/UnitConstants.h"
#include "geos/geom/Envelope.h"

using namespace geos::geom;

class GSSEnvelope : public QObject
{
    Q_OBJECT

public:
    explicit GSSEnvelope(QObject *parent = 0);
    GSSEnvelope( double minX , double maxX , double minY , double maxY , QObject *parent = 0);
    GSSEnvelope( GSSCoordinate c1 , GSSCoordinate c2 , QObject *parent = 0);
    GSSEnvelope( const GSSEnvelope& );

    // EXPORTERS
    QJsonObject toGeoJSON( QJsonObject properties = QJsonObject() ) const;
    QImage toImage( const GSSEnvelope image_bounds , int image_width = 1024 , int image_height = 1024 , QColor border_color = QColor("black") , int border_width = 10 , QColor fill_color = QColor("green") ) const;
    QString toString() const;

    // GETTERS
    double getMaxX() const;
    double getMinX() const;
    double getMaxY() const;
    double getMinY() const;
    GSSCoordinate getCentroid() const;
    AreaUnit getArea() const;
    bool covers( GSSEnvelope env ) const;
    bool covers( GSSCoordinate coor ) const;
    bool contains( GSSCoordinate coor ) const;
    bool intersects( GSSEnvelope env ) const;

    // METHODS
    GSSEnvelope createBuffer(double buffer) const;
    GSSEnvelope createUnion(GSSEnvelope other) const;

    // OPERATORS
    GSSEnvelope& operator= (const GSSEnvelope&);
    bool operator == (const GSSEnvelope&) const;
    bool operator != (const GSSEnvelope&) const;

private:
    geos::geom::Envelope inner_envelope;
};

#endif // GSSENVELOPE_H
