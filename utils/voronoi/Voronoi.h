#ifndef VORONOI_H
#define VORONOI_H

#include <QObject>

#include "utils/geometry/GSSCoordinate.h"
#include "utils/geometry/GSSEnvelope.h"
#include "utils/geometry/GSSGeometryController.h"

class Voronoi : public QObject
{
    Q_OBJECT

public:
    explicit Voronoi(QObject *parent = 0);


    // METHODS
    QList<GSSGeometry*> createVoronoi( QList<GSSCoordinate> coordinates , const GSSEnvelope envelope , const GSSGeometryController* geometry_controller );
};

#endif // VORONOI_H
