#ifndef GSSLINESTRING_H
#define GSSLINESTRING_H

#include "utils/geometry/GSSGeometry.h"
#include "geos/geom/LineString.h"

class GSSLineString : public GSSGeometry
{
    Q_OBJECT
    friend class GSSGeometryController; // GeometryController can access this constructor
    friend class GSSGeometryUtils;
    friend class GSSGeometry;

public:

    // EXPORTERS
    virtual QImage toImage(const GSSEnvelope image_bounds, int image_width = 1024, int image_height = 1024, QColor border_color = QColor("Black"), int border_width = 10 , QColor fill_color = QColor("Green"), QImage icon = QImage()) const;
    virtual QJsonObject toGeoJSON(QJsonObject properties) const;

    // GETTERS
    LengthUnit getLength() const;

    // METHODS (Caller takes ownership of returned geometry)
    GSSLineString* createReversed() const;

protected:
    explicit GSSLineString( geos::geom::LineString* geom , QObject *parent = 0);

};

#endif // GSSLINESTRING_H
