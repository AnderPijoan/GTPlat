#ifndef PROJECTIONCONVERSOR_H
#define PROJECTIONCONVERSOR_H

#include <QObject>
#include <QDebug>
#include <utils/geometry/GSSGeometryController.h>

#include "proj_api.h"

class ProjectionConversor : public QObject
{
    Q_OBJECT
public:
    static ProjectionConversor* getInstance();

    // METHODS
    GSSCoordinate convert( double x , double y, int from_epsg_code , int to_epsg_code );
    GSSCoordinate convert( GSSCoordinate coor , int from_epsg_code , int to_epsg_code);
    double getDistanceInMeters(double lon1, double lat1, double lon2, double lat2);

private:
    ProjectionConversor();
    ProjectionConversor(ProjectionConversor const&);
    ~ProjectionConversor();

    QMap< int , char * > proj_commands;
};

#endif // PROJECTIONCONVERSOR_H
