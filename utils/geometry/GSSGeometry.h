#ifndef GSSGEOMETRY_H
#define GSSGEOMETRY_H

#include <QObject>
#include "geos/geom/Geometry.h"

#include <QJsonObject>
#include <QImage>
#include <QColor>

#include "utils/units/UnitConstants.h"
#include "utils/geometry/GSSCoordinate.h"
#include "utils/geometry/GSSEnvelope.h"

QT_FORWARD_DECLARE_CLASS(GSSPoint)

using namespace geos::geom;

class GSSGeometry : public QObject
{
    Q_OBJECT
    friend class GSSGeometryController; // GeometryController can access this constructor
    friend class GSSGeometryUtils;

public:
    ~GSSGeometry();

    // EXPORTERS
    virtual QImage toImage(const GSSEnvelope image_bounds, int image_width = 1024, int image_height = 1024, QColor border_color = QColor("Black"), int border_width = 10 , QColor fill_color = QColor("Green"), QImage icon = QImage()) const;
    virtual QJsonObject toGeoJSON( QJsonObject properties = QJsonObject() ) const;
    virtual QString toString() const;

    // GETTERS
    bool isValid() const;
    bool intersects( const GSSGeometry *other) const;
    bool equals( const GSSGeometry* other ) const;
    GSSCoordinate getRepresentativeCoordinate() const;
    GSSEnvelope getEnvelope() const;
    QList<GSSCoordinate> getCoordinates() const;
    virtual AreaUnit getArea() const;
    virtual double getDistance( const GSSGeometry* other ) const;
    virtual GSSCoordinate getFirstCoordinate() const;
    virtual GSSCoordinate getLastCoordinate() const;

    // SETTERS

    // METHODS (Caller takes ownership of returned geometry)
    virtual GSSGeometry* createCopy() const;
    GSSPoint* createCentroid() const;
    GSSEnvelope createEnvelope() const;
    GSSGeometry* createUnion( GSSGeometry* other ) const;
    GSSGeometry* createIntersection( const GSSGeometry* other ) const;

protected:
    explicit GSSGeometry( geos::geom::Geometry* geom , QObject *parent = 0);

    const geos::geom::Geometry* inner_geometry = 0;

private:
    GSSGeometry* castToGeometryType( geos::geom::Geometry* geom ) const;
};

#endif // GSSGEOMETRY_H
