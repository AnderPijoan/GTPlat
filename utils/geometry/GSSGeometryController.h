#ifndef GSSGEOMETRYCONTROLLER_H
#define GSSGEOMETRYCONTROLLER_H

#include <QObject>

#include "geos/geom/GeometryFactory.h"

#include "utils/geometry/GSSCoordinate.h"
#include "utils/geometry/GSSEnvelope.h"
#include "utils/geometry/GSSGeometry.h"
#include "utils/geometry/GSSPoint.h"
#include "utils/geometry/GSSLineString.h"
#include "utils/geometry/GSSPolygon.h"
#include "utils/grid/GSSGrid.h"

class GSSGeometryController : public QObject
{
    Q_OBJECT
    friend class PhysicalEnvironment; // PhysicalEnvironment can access this constructor

public:

    explicit GSSGeometryController(QObject *parent = 0);
    ~GSSGeometryController();

    // GETTERS
    const geos::geom::GeometryFactory* getGeometryFactory() const;
    const GSSGrid getElevationModel() const;
    GSSCoordinate getRandomPoint( const GSSGeometry* bounds , unsigned int seed = qrand() ) const;

    // CREATORS
    GSSGeometry* createGeometry(QString wkt , bool elevate = true ) const;

    GSSPoint* createPoint( GSSCoordinate coor , bool elevate = true ) const;
    GSSPoint* createPoint( double x , double y , double z = 0 , bool elevate = true ) const;

    GSSLineString* createLineString( GSSCoordinate c1 , GSSCoordinate c2 , bool elevate = true ) const;
    GSSLineString* createLineString( QList<GSSCoordinate> coors , bool elevate = true ) const;

    GSSPolygon* createPolygon( QList< QList<GSSCoordinate> > rings_coors , bool elevate = true ) const;

    // ELEVATION SETTERS
    double getElevation( GSSCoordinate coor ) const;
    void setElevationModel( const GSSGrid& elevation );
    void setElevationPoint( GSSCoordinate coor , double value );

private:
    // SUBCREATORS
    GSSPoint* createPoint( geos::geom::Point* gpoint , bool elevate = true ) const;
    GSSLineString* createLineString( geos::geom::LineString* gline , bool elevate = true ) const;
    GSSPolygon* createPolygon( geos::geom::Polygon* gpolygon , bool elevate = true ) const;

    GSSGrid elevation_model; // Elevation model IN METERS
    geos::geom::GeometryFactory* geometry_factory; // Geometry factory
};

#endif // GSSGEOMETRYCONTROLLER_H
