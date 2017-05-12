#ifndef GSSGEOMETRYUTILS_H
#define GSSGEOMETRYUTILS_H

#include <QObject>
#include "utils/geometry/GSSGeometry.h"
#include "utils/geometry/GSSGeometryController.h"

class GSSGeometryUtils : public QObject
{
    Q_OBJECT

public:
    static GSSGeometryUtils* getInstance();

    GSSGeometry* simplifyDouglasPeucker(GSSGeometry* geom, int distance_tolerance);
    GSSGeometry* reprojectGeometry(const GSSGeometry* geom, const GSSGeometryController* controller, int from_epsg_code , int to_epsg_code);
    AreaUnit getGeometryArea( const GSSGeometry* geom, const GSSGeometryController* controller );
    GSSGeometry* getIntersectingPolygonFromMultipolygon( const GSSGeometry* multipolygon, const GSSGeometry* geom, const GSSGeometryController* controller );

private:
    explicit GSSGeometryUtils(QObject *parent = 0);
    GSSGeometryUtils(GSSGeometryUtils const&);
    ~GSSGeometryUtils();

};

#endif // GSSGEOMETRYUTILS_H
