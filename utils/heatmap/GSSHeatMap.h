#ifndef GSSHEATMAP_H
#define GSSHEATMAP_H

#include <QMap>

#include "utils/heatmap/GSSHeatMapPalette.h"
#include "utils/geometry/GSSEnvelope.h"
#include "utils/geometry/GSSCoordinate.h"

class GSSHeatMap
{

public:
    explicit GSSHeatMap();
    ~GSSHeatMap();

    // EXPORTERS
    virtual QJsonObject toJSON();
    virtual QImage toImage(const GSSEnvelope image_bounds, int image_width = 1024, int image_height = 1024) const;

    // SETTERS
    void addPoint( const GSSCoordinate &point , double weight = 1);

protected:
    QMap<GSSCoordinate , double> points;
    double max_value;

    // Color palette gradient
    GSSHeatMapPalette palette;

};

#endif // GSSHEATMAP_H
