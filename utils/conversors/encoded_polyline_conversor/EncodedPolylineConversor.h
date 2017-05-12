#ifndef ENCODEDPOLYLINECONVERSOR_H
#define ENCODEDPOLYLINECONVERSOR_H

#include <QObject>
#include <QDebug>

#include "geos/geom/GeometryFactory.h"
#include "geos/geom/Point.h"

using namespace geos::geom;

class EncodedPolylineConversor: public QObject
{
    Q_OBJECT
public:
    explicit EncodedPolylineConversor(QObject *parent = 0);

    // METHODS
    QList<geos::geom::Point*> decodePolyline( geos::geom::GeometryFactory* factory , QString encoded);

private:
    double PRECISION = 1E5;

};

#endif // ENCODEDPOLYLINECONVERSOR_H
