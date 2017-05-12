#ifndef GEOJSONTOGEOMETRY_H
#define GEOJSONTOGEOMETRY_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

#include "utils/geometry/GSSGeometryController.h"
#include "geos/geom/Geometry.h"

using namespace geos::geom;

class GeoJsonToGeometry : public QObject
{
    Q_OBJECT

public:
    static GSSGeometry* format( const GSSGeometryController* environment , QJsonObject geojson );
};

#endif // GEOJSONTOGEOMETRY_H
