#include "GSSGeometryUtils.h"

#include "geos/geom/Point.h"
#include "geos/geom/LineString.h"
#include "geos/geom/Polygon.h"
#include "geos/simplify/DouglasPeuckerSimplifier.h"
#include "utils/conversors/projection_conversor/ProjectionConversor.h"
#include "utils/exporters/debugger/Debugger.h"

GSSGeometryUtils* GSSGeometryUtils::getInstance(){
    static GSSGeometryUtils instance;
    return &instance;
}

GSSGeometryUtils::GSSGeometryUtils(QObject *parent) : QObject(parent){
}

GSSGeometryUtils::~GSSGeometryUtils(){
}

/**********************************************************************
 METHODS
**********************************************************************/

GSSGeometry* GSSGeometryUtils::simplifyDouglasPeucker(GSSGeometry* geom, int distance_tolerance){
    geos::simplify::DouglasPeuckerSimplifier* simplifier = new geos::simplify::DouglasPeuckerSimplifier( geom->inner_geometry );
    simplifier->setDistanceTolerance( distance_tolerance );
    return new GSSGeometry( simplifier->getResultGeometry().release() );
}

GSSGeometry* GSSGeometryUtils::reprojectGeometry(const GSSGeometry* geom, const GSSGeometryController* controller, int from_epsg_code , int to_epsg_code){

    try {
        // POINT
        if( geom->inner_geometry->getGeometryTypeId() == GEOS_POINT){
            const GSSPoint* point = dynamic_cast<const GSSPoint*>(geom);
            return controller->createPoint( ProjectionConversor::getInstance()->convert( point->getRepresentativeCoordinate(), from_epsg_code, to_epsg_code ) );

        // LINESTRING
        } else if( geom->inner_geometry->getGeometryTypeId() == GEOS_LINESTRING ){
            const GSSLineString* line = dynamic_cast<const GSSLineString*>(geom);
            QList<GSSCoordinate> coordinates;

            for(unsigned int i = 0; i < line->getCoordinates().size(); i++){
                GSSCoordinate coord = line->getCoordinates().at( i );
                coordinates.append( ProjectionConversor::getInstance()->convert( coord, from_epsg_code, to_epsg_code ) );
            }
            return controller->createLineString(coordinates);

        // POLYGON
         } else if( geom->inner_geometry->getGeometryTypeId() == GEOS_POLYGON ){
            const geos::geom::Polygon* polygon = dynamic_cast<const geos::geom::Polygon*>( geom->inner_geometry );

            QList< QList<GSSCoordinate> > rings;
            QList<GSSCoordinate> rings_coors;

            for(unsigned int i = 0; i < polygon->getExteriorRing()->getCoordinates()->size(); i++){
                geos::geom::Coordinate original_coord = 0;
                original_coord.x = polygon->getExteriorRing()->getCoordinates()->getAt( i ).x;
                original_coord.y = polygon->getExteriorRing()->getCoordinates()->getAt( i ).y;
                rings_coors.append( ProjectionConversor::getInstance()->convert( GSSCoordinate(original_coord.x, original_coord.y), from_epsg_code, to_epsg_code ) );
            }
            rings.append(rings_coors);

            for(unsigned int i = 0; i < polygon->getNumInteriorRing(); i++){
                QList<GSSCoordinate> rings_coors;

                for(unsigned int j = 0; j < polygon->getInteriorRingN( i )->getCoordinates()->size(); j++){
                    geos::geom::Coordinate original_coord = 0;
                    original_coord.x = polygon->getInteriorRingN( i )->getCoordinates()->getAt( j ).x;
                    original_coord.y = polygon->getInteriorRingN( i )->getCoordinates()->getAt( j ).y;
                    rings_coors.append( ProjectionConversor::getInstance()->convert( GSSCoordinate(original_coord.x, original_coord.y), from_epsg_code, to_epsg_code ) );
                }
                rings.append(rings_coors);
            }
            return controller->createPolygon( rings );

        } else {
            Debugger::warning( this, Q_FUNC_INFO,  QString( "Unknown geometry type: %1").arg( QString::fromStdString( geom->inner_geometry->getGeometryType() ) ) );
        }

    } catch ( ... ){        
        Debugger::critical( this , Q_FUNC_INFO , QString("Geometry type parsing crashed" ) );
    }
}

AreaUnit GSSGeometryUtils::getGeometryArea( const GSSGeometry* geom, const GSSGeometryController* controller ){
   return this->reprojectGeometry( geom, controller, 4326 , 23030 )->getArea();
}

GSSGeometry* GSSGeometryUtils::getIntersectingPolygonFromMultipolygon( const GSSGeometry* multipolygon, const GSSGeometry* geom, const GSSGeometryController* controller ){
     geos::geom::Geometry* intersecting_geom;

     for( unsigned int i = 0; i < multipolygon->inner_geometry->getNumGeometries(); i++ ){
         if ( multipolygon->inner_geometry->getGeometryN( i )->intersects( geom->inner_geometry ) ){
             intersecting_geom = multipolygon->inner_geometry->getGeometryN( i )->clone();
             break;
         }
     }
     return controller->createGeometry( QString::fromStdString( intersecting_geom->toString() ) );
}
