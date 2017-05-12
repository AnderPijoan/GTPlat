#include "ProjectionConversor.h"

//#include "geodesic.h"
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/LineString.h>


ProjectionConversor* ProjectionConversor::getInstance(){
    static ProjectionConversor instance;
    return &instance;
}

ProjectionConversor::ProjectionConversor() : QObject(){

    // Load commands
    this->proj_commands.insert( 4326 , "+proj=longlat +datum=WGS84 +no_defs" );
    this->proj_commands.insert( 23030 , "+proj=utm +zone=30 +ellps=intl +towgs84=-87,-98,-121,0,0,0,0 +units=m +no_defs " );
    this->proj_commands.insert( 900913 , "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +no_defs " );
    this->proj_commands.insert( 25830 , "+proj=utm +zone=30 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs ");
    this->proj_commands.insert( 3035 , "+proj=laea +lat_0=52 +lon_0=10 +x_0=4321000 +y_0=3210000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs " );
    this->proj_commands.insert( 3042 , "+proj=utm +zone=30 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs " );
}

ProjectionConversor::~ProjectionConversor(){
}

/**********************************************************************
 METHODS
**********************************************************************/

GSSCoordinate ProjectionConversor::convert(double x, double y, int from_epsg_code , int to_epsg_code){
    return this->convert( GSSCoordinate( x , y ), from_epsg_code, to_epsg_code );
}

GSSCoordinate ProjectionConversor::convert(GSSCoordinate coor, int from_epsg_code , int to_epsg_code){

    if( !this->proj_commands.keys().contains( from_epsg_code ) || !this->proj_commands.keys().contains( to_epsg_code ) ){
        qWarning() << Q_FUNC_INFO << "Source or destination projections not existing";
        return coor;
    }

    double newx = coor.getX();
    double newy = coor.getY();

    projPJ from = pj_init_plus( this->proj_commands.value( from_epsg_code ) );
    if( !from ){
        qWarning() << Q_FUNC_INFO << "Error intializing projection:" << from_epsg_code;
    }

    projPJ to = pj_init_plus( this->proj_commands.value( to_epsg_code ) );
    if( !to ){
        qWarning() << Q_FUNC_INFO << "Error intializing projection:" << to_epsg_code;
    }

    if( pj_is_latlong( from ) ){
        newx *= DEG_TO_RAD;
        newy *= DEG_TO_RAD;
    }

    // Do transformation between projections
    int error_code = pj_transform( from , to , 1 , 1 , &newx, &newy, NULL );

    if( error_code != 0 ){
        qWarning() << Q_FUNC_INFO << "Error in conversion from projections" << from <<  "to" << to << ":" << pj_strerrno( error_code ) ;
    }

    // If TO projection is latlong, convert from RAD to DEG
    if( pj_is_latlong( to ) ){
        newx *= RAD_TO_DEG;
        newy *= RAD_TO_DEG;
    }

    return GSSCoordinate(newx, newy);
}


double ProjectionConversor::getDistanceInMeters(double lon1, double lat1, double lon2, double lat2){
    return 0;
}

