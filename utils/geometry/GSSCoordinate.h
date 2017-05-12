#ifndef GSSCOORDINATE_H
#define GSSCOORDINATE_H

#include <QObject>
#include "geos/geom/Coordinate.h"

using namespace geos::geom;

class GSSCoordinate : public QObject
{
    Q_OBJECT
    friend class GSSGeometryController;
    friend class GSSGraphNode;
    friend class GSSGraphEdge;

public:
    explicit GSSCoordinate(QObject *parent = 0);
    GSSCoordinate( double x , double y , double z = 0 , QObject *parent = 0);
    GSSCoordinate( const GSSCoordinate& );
    ~GSSCoordinate();

    // EXPORTERS
    QString toString() const;

    // GETTERS
    bool isValid() const;
    double getX() const;
    double getY() const;
    double getZ() const;
    double distance( GSSCoordinate other ) const;

    // OPERATORS
    bool operator == (const GSSCoordinate&) const;
    bool operator != (const GSSCoordinate&) const;
    bool operator < (const GSSCoordinate&) const;
    GSSCoordinate& operator= (const GSSCoordinate&);

private:
    geos::geom::Coordinate inner_coordinate;
};

#endif // GSSCOORDINATE_H
