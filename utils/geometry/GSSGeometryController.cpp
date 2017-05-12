#include "GSSGeometryController.h"

#include "geos/geom/PrecisionModel.h"
#include "geos/geom/CoordinateSequenceFactory.h"
#include "geos/geom/Envelope.h"
#include "geos/geom/LinearRing.h"
#include "geos/geom/Point.h"
#include "geos/io/WKTReader.h"

#include "utils/exporters/debugger/Debugger.h"
#include "utils/random_generators/UniformDistribution.h"

GSSGeometryController::GSSGeometryController(QObject *parent) : QObject(parent) {
    // Create geometry factory
    PrecisionModel* precision_model = new PrecisionModel( PrecisionModel::FLOATING );
    this->geometry_factory = new GeometryFactory( precision_model, 4326 );
}

GSSGeometryController::~GSSGeometryController(){
    delete this->geometry_factory;
}

/**********************************************************************
 GETTERS
**********************************************************************/

const GSSGrid GSSGeometryController::getElevationModel() const{
    return this->elevation_model;
}

const geos::geom::GeometryFactory* GSSGeometryController::getGeometryFactory() const{
    return this->geometry_factory;
}

GSSCoordinate GSSGeometryController::getRandomPoint( const GSSGeometry *bounds, unsigned int seed) const{
    if( bounds->getCoordinates().size() <= 2 ){
        return bounds->getRepresentativeCoordinate();
    }

    srand( seed );
    GSSCoordinate result;

    int max_attemps = 4;
    do {

        double x_diff = bounds->getEnvelope().getMaxX() - bounds->getEnvelope().getMinX();
        double y_diff = bounds->getEnvelope().getMaxY() - bounds->getEnvelope().getMinY();
        double x_incr = (( qrand() * 100000 ) % int(x_diff * 100000) ) / 100000;
        double y_incr = (( qrand() * 100000 ) % int(y_diff * 100000) ) / 100000;

        geos::geom::Point* p = this->geometry_factory->createPoint( geos::geom::Coordinate( bounds->getEnvelope().getMinX() + x_incr ,  bounds->getEnvelope().getMinY() + y_incr , 0 ) );

        if( bounds->inner_geometry->contains( p ) ){
            result = GSSCoordinate( p->getCoordinate()->x , p->getCoordinate()->y , p->getCoordinate()->z );
        }
        delete p;

        max_attemps--;
    } while( max_attemps > 0);

    if( max_attemps <= 0 ){
        return bounds->getRepresentativeCoordinate();
    }

    return result;
}

/**********************************************************************
 CREATORS
**********************************************************************/

GSSGeometry* GSSGeometryController::createGeometry(QString wkt , bool elevate) const{

    try{
        // Create reader to build geometry without our elevation
        geos::io::WKTReader reader = geos::io::WKTReader( this->geometry_factory );
        geos::geom::Geometry* g = reader.read( wkt.trimmed().toStdString() );

        // POINT
        if( g->getGeometryTypeId() == geos::geom::GEOS_POINT ){
            return this->createPoint( dynamic_cast<geos::geom::Point*>( g ) , elevate );
        }

        // LINESTRING
        if( g->getGeometryTypeId() == geos::geom::GEOS_LINESTRING ){
            return this->createLineString( dynamic_cast<geos::geom::LineString*>( g ) , elevate );
        }

        // POLYGON
        if( g->getGeometryTypeId() == geos::geom::GEOS_POLYGON ){
            return this->createPolygon( dynamic_cast<Polygon*>( g ) , elevate );
        }

        return new GSSGeometry( g );
    } catch(...){
    }
    return 0;
}

// POINT

GSSPoint* GSSGeometryController::createPoint( geos::geom::Point* gpoint, bool elevate) const{
    if( !elevate ){
        return new GSSPoint( gpoint );
    }
    double x = gpoint->getCoordinate()->x;
    double y = gpoint->getCoordinate()->y;
    double z = gpoint->getCoordinate()->z;
    delete gpoint;
    return this->createPoint( x , y , z , elevate );
}


GSSPoint* GSSGeometryController::createPoint(double x, double y, double z, bool elevate) const{
    return this->createPoint( GSSCoordinate( x , y , z ) , elevate );
}

GSSPoint* GSSGeometryController::createPoint(GSSCoordinate c , bool elevate) const{
    return new GSSPoint( this->geometry_factory->createPoint( geos::geom::Coordinate( c.getX() , c.getY() , elevate ? this->getElevation( c ) : c.getZ() ) ) );
}

// LINESTRING

GSSLineString* GSSGeometryController::createLineString( geos::geom::LineString *gline, bool elevate) const{
    if( !elevate ){
        return new GSSLineString( gline );
    }
    QList<GSSCoordinate> coors;
    for(unsigned int i = 0; i < gline->getCoordinates()->size(); i++){
        coors.append( GSSCoordinate( gline->getCoordinateN( i ).x , gline->getCoordinateN( i ).y ) );
    }
    delete gline;
    return this->createLineString( coors , elevate );
}

GSSLineString* GSSGeometryController::createLineString(GSSCoordinate c1, GSSCoordinate c2 , bool elevate) const{
    QList<GSSCoordinate> coors;
    coors.append( c1 );
    coors.append( c2 );
    return this->createLineString( coors , elevate );
}

GSSLineString* GSSGeometryController::createLineString(QList<GSSCoordinate> coors , bool elevate) const{
    if( coors.isEmpty() || coors.size() == 1){
        Debugger::warning( this , Q_FUNC_INFO , "Empty or single coordinate linestring" );
        return 0;
    }
    CoordinateSequence* seq = this->geometry_factory->getCoordinateSequenceFactory()->create( (std::size_t)0 );
    foreach(GSSCoordinate c , coors){
        seq->add( geos::geom::Coordinate( c.getX() , c.getY() , elevate ? this->getElevation( c ) : c.getZ() ) );
    }
    return new GSSLineString( this->geometry_factory->createLineString( seq ) );
}

// POLYGON

GSSPolygon* GSSGeometryController::createPolygon( geos::geom::Polygon *gpolygon, bool elevate) const{
    if( !elevate ){
        return new GSSPolygon( gpolygon );
    }

    QList<QList<GSSCoordinate>> rings_coors;
    const geos::geom::LineString* out_ring = gpolygon->getExteriorRing();

    QList<GSSCoordinate> out_coors;
    for(unsigned int i = 0 ; i < out_ring->getCoordinates()->size() ; i++){
        out_coors.append( GSSCoordinate( out_ring->getCoordinateN( i ).x , out_ring->getCoordinateN( i ).y , out_ring->getCoordinateN( i ).z ) );
    }
    rings_coors.append( out_coors );

    for(unsigned int i = 0; i < gpolygon->getNumInteriorRing(); i++){
        const geos::geom::LineString* in_ring = gpolygon->getInteriorRingN( i );

        QList<GSSCoordinate> in_coors;
        for(unsigned int i = 0 ; i < in_ring->getCoordinates()->size() ; i++){
            in_coors.append( GSSCoordinate( in_ring->getCoordinateN( i ).x , in_ring->getCoordinateN( i ).y , in_ring->getCoordinateN( i ).z ) );
        }
        rings_coors.append( in_coors );
    }
    delete gpolygon;
    return this->createPolygon( rings_coors , elevate );
}

GSSPolygon* GSSGeometryController::createPolygon(QList<QList<GSSCoordinate> > rings_coors , bool elevate) const{
    LinearRing* shell = 0;
    std::vector<geos::geom::Geometry*> holes;

    foreach(QList<GSSCoordinate> ring_coors , rings_coors){
        CoordinateSequence* coordinates = this->geometry_factory->getCoordinateSequenceFactory()->create( (std::size_t)0 );
        foreach(GSSCoordinate c , ring_coors){
            coordinates->add( geos::geom::Coordinate( c.getX() , c.getY() , elevate ? this->getElevation( c ) : c.getZ() ) );
        }

        // First Coordinate list is the outer ring/shell
        if( !shell ){
            shell = this->geometry_factory->createLinearRing( coordinates );
        } else {
            LinearRing* r = this->geometry_factory->createLinearRing( coordinates );
            holes.push_back( r );
        }
    }
    return new GSSPolygon( this->geometry_factory->createPolygon( *shell , holes ) );
}

/**********************************************************************
 ELEVATION
**********************************************************************/

double GSSGeometryController::getElevation( GSSCoordinate coor) const{
    return this->elevation_model.getValue( coor );
}

void GSSGeometryController::setElevationPoint( GSSCoordinate coor, double value ){
    this->elevation_model.setValue( coor , value );
}

void GSSGeometryController::setElevationModel( const GSSGrid& elevation){
    this->elevation_model = elevation;
}
