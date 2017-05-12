#include <QCoreApplication>
#include <QDir>
#include <QSqlRecord>
#include <QJsonDocument>
#include <QProcess>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>

#include "environment/Environment.h"

#include "agents/places/administrative_areas/AdministrativeArea.h"
#include "agents/places/constructions/accommodations/Accommodation.h"
#include "agents/places/constructions/civil_structures/vehicle_rentals/BicycleRental.h"
#include "agents/places/constructions/civil_structures/parkings/Parking.h"
#include "agents/places/constructions/civil_structures/charging_stations/ChargingStation.h"
#include "agents/places/constructions/civil_structures/public_transport_stops/BusStop.h"
#include "agents/places/constructions/civil_structures/public_transport_stops/RailwayStop.h"
#include "agents/transport_networks/elements/TrafficLight.h"
#include "agents/transport_networks/railways/Railway.h"
#include "agents/transport_networks/roads/Road.h"
#include "agents/transport_networks/walk_paths/WalkPath.h"
#include "agents/vehicles/public_transport/Bus.h"
#include "agents/vehicles/public_transport/Ferry.h"
#include "agents/vehicles/public_transport/Funicular.h"
#include "agents/vehicles/public_transport/Metro.h"
#include "agents/vehicles/public_transport/Train.h"
#include "agents/vehicles/public_transport/Tram.h"

#include "utils/db/PostgresDriver.h"
#include "utils/exporters/debugger/Debugger.h"
#include "utils/conversors/projection_conversor/ProjectionConversor.h"
#include "utils/importers/tiff_importer/TiffImporter.h"
#include "utils/importers/gtfs_importer/GtfsImporter.h"
#include "utils/http_server/HttpServer.h"
#include "utils/tile_server/TileServer.h"
#include "utils/websocket_server/WebSocketServer.h"
#include "utils/performance/PerformanceMonitor.h"
#include "utils/exporters/logger/Logger.h"
#include "utils/importers/ine_importer/IneMicroDataImporter.h"
#include "utils/graph/GSSGraphUtils.h"

#include "usecases/gtplat/GTPlatScenarioCreator.h"


void exec_forever( QCoreApplication* app ){
    try{
        app->exec();
    } catch(...){
        qDebug() << "AAAAA";
        exec_forever( app );
    }
}

int main(int argc, char *argv[])
{
    // CREATE QAPPLICATION
    QCoreApplication* app = new QCoreApplication( argc , argv );
    app->thread()->setPriority( QThread::TimeCriticalPriority );

    /*qmlRegisterType<Agent>("com.mycompany.agent", 1, 0, "Agent");

    QQmlEngine engine;
    QQmlComponent component( &engine , "/home/ander/Workspace/GeoSmartSim/agent.qml" );
    QObject* obj = component.create();
    qDebug() << obj->property("name");
    Agent* obj_a = dynamic_cast<Agent*>( obj );
    qDebug() << obj_a;*/

    // CREATE ENVIRONMENT
    Environment * environment = new Environment( app );
    environment->setGeometry(
                environment->getPhysicalEnvironment()->getGeometryController()->createGeometry(
                    QString("POLYGON EMPTY") ) ); // INITIAL

    const GSSGeometry* environment_geometry = environment->getGeometry();

    /*********************************/
    //
    // GREENTRAVELLING SIMULATOR - GTPLAT
    //
    /*********************************/

    // CREATE INIT PARAMS
    {
        QDir folder("./../usecases/gtplat/scenario/init");
        QFile file( folder.absoluteFilePath( "init.json" ) );
        if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {

            QJsonParseError jerror;
            QJsonDocument json_document = QJsonDocument::fromJson( file.readAll() , &jerror);

            if(jerror.error != QJsonParseError::NoError){
                Debugger::critical( 0 , Q_FUNC_INFO , QString("Error when parsing init JSON: %1").arg( jerror.errorString() ) );
                return -1;
            }

            QJsonObject json = json_document.object();
            QTime t = QTime::fromString( json.value("time").toString() , "hh:mm:ss" );
            QDate d = QDate::fromString( json.value("date").toString() , "yyyy-MM-dd" );
            const_cast<TimeEnvironment*>( environment->getTimeEnvironment() )->setDate( d );
            const_cast<TimeEnvironment*>( environment->getTimeEnvironment() )->setTime( t );
            const_cast<TimeEnvironment*>( environment->getTimeEnvironment() )->setTimeSpeed( json.value("speed").toInt( 1 ) );

            app->setProperty( "simulation_hours" , json.value("simulation_hours").toInt( 24 ) );

        } else {

            Debugger::critical( 0 , Q_FUNC_INFO , "NO INIT FILE FOUND" );
            app->exit();
            return -1;

        }
    }

    // CREATE DTABASE DRIVER
    PostgresDriver* db = 0;

    QDir postgis("./../usecases/gtplat/scenario/postgis");
    QFile file( postgis.absoluteFilePath( "postgis.json" ) );
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QJsonParseError jerror;
        QJsonDocument json_document = QJsonDocument::fromJson( file.readAll() , &jerror);

        if(jerror.error != QJsonParseError::NoError){
            Debugger::critical( 0 , Q_FUNC_INFO , QString("Error when parsing postgis JSON: %1").arg( jerror.errorString() ) );
            return -1;
        }

        QJsonObject json = json_document.object();

        app->setProperty( "postgis-host" , json.value("host").toString() );
        app->setProperty( "postgis-port" , json.value("port").toInt( 5432 ) );
        app->setProperty( "postgis-database" , json.value("database").toString() );
        app->setProperty( "postgis-user" , json.value("user").toString() );
        app->setProperty( "postgis-password" , json.value("password").toString() );

        app->setProperty( "postgis-polygons" , json.value("polygons").toString("planet_osm_polygon") );
        app->setProperty( "postgis-polygons_geom" , json.value("polygons_geom").toString("way") );
        app->setProperty( "postgis-polygons_tags" , json.value("polygons_tags").toString("tags") );

        app->setProperty( "postgis-lines" , json.value("lines").toString("planet_osm_line") );
        app->setProperty( "postgis-lines_geom" , json.value("lines_geom").toString("way") );
        app->setProperty( "postgis-lines_tags" , json.value("lines_tags").toString("tags") );

        app->setProperty( "postgis-points" , json.value("points").toString("planet_osm_point") );
        app->setProperty( "postgis-points_geom" , json.value("points_geom").toString("way") );
        app->setProperty( "postgis-points_tags" , json.value("points_tags").toString("tags") );

        db = new PostgresDriver( app->property("postgis-host").toString() , app->property("postgis-port").toInt() , app->property("postgis-database").toString() , app->property("postgis-user").toString() , app->property("postgis-password").toString() );

    } else {

        Debugger::critical( 0 , Q_FUNC_INFO , "NO POSTGIS FILE FOUND" );
        app->exit();
        return -1;

    }

    // LOAD ELEVATION
    //TiffImporter* tiff = new TiffImporter("./../usecases/gtplat/scenario/elevation/srtm_36_04.tif");
    //const_cast<GSSGeometryController*>( environment->getPhysicalEnvironment()->getGeometryController() )->setElevationModel( tiff->getValues() ); // In meters
    //tiff->deleteLater();

    // LOAD BOUNDARIES FROM SCENARIO FOLDER
    Debugger::info( 0 , Q_FUNC_INFO , "Creating boundaries");

    QDir boundaries("./../usecases/gtplat/scenario/boundaries");
    foreach( QString file_name , boundaries.entryList() ){

        QFile file( boundaries.absoluteFilePath( file_name ) );

        if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            GSSGeometry* geom = environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( in.readAll() );

            if( !geom ){
                continue;
            }

            if( !geom->isValid() ){
                Debugger::warning( 0 , Q_FUNC_INFO , QString("Invalid geometry in file %1").arg( file_name ) );
                delete geom;
                continue;
            }

            environment->setGeometry( environment_geometry->createUnion( geom ) );
            environment_geometry = environment->getGeometry();

            // Create land
            AdministrativeArea* boundary = new AdministrativeArea( geom );
            boundary->setObjectName( file_name.split(".").at(0) );
            environment->addAgent( boundary );
            Debugger::info( 0 , Q_FUNC_INFO , QString("Created Boundary %1").arg( boundary->objectName() ) );

            file.close();
        }
    }


    // LOAD HOUSES FROM DATABASE
    Debugger::info( 0 , Q_FUNC_INFO , "Creating buildings");
    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( %2 ) AS st_astext, \
                                ST_Area( %2::geography ) AS st_area, \
                                name AS name, \
                                %3->'building:levels' AS levels \
                                FROM \
                                %1 \
                                WHERE \
                                (%3 @> '\"building\"=>\"residential\"' OR \
                                %3 @> '\"building\"=>\"house\"' OR \
                                %3 @> '\"building\"=>\"yes\"' OR \
                                %3 @> '\"building\"=>\"apartments\"' \
                                ) AND \
                                ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) ")
                        .arg( app->property("postgis-polygons").toString() )
                        .arg( app->property("postgis-polygons_geom").toString() )
                        .arg( app->property("postgis-polygons_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try{

                GSSGeometry* geom = environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() );

                Accommodation* building = new Accommodation( geom );
                building->setLevels( qMax( 1 , record.value("levels").toInt() ) );
                building->setArea( record.value("st_area").toDouble() );
                environment->addAgent( building );

            } catch(...){}
        }
    }

    // LOAD BUS STOPS FROM DATABASE
    Debugger::info( 0 , Q_FUNC_INFO , "Creating bus stops" );

    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( %2 ) AS st_astext, \
                                name AS name \
                                FROM \
                                %1 \
                                WHERE \
                                (%3 @> '\"highway\"=>\"bus_stop\"' OR \
                                %3 @> '\"amenity\"=>\"bus_station\"') AND \
                                ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) ")
                        .arg( app->property("postgis-points").toString() )
                        .arg( app->property("postgis-points_geom").toString() )
                        .arg( app->property("postgis-points_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try {
                GSSPoint* geom = dynamic_cast<GSSPoint*>( environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() ) );

                // Create stop
                BusStop* stop = dynamic_cast<BusStop*>( environment->getNetworkEnvironment()->getNodeFromGraph( geom->getRepresentativeCoordinate() , BusStop::staticMetaObject.className() ) );
                if( !stop ){
                    stop = new BusStop( geom );
                    stop->setObjectName( record.value("name").toString() );
                    environment->addAgent( stop );
                }

            } catch(...){}
        }
    }

    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( %2 ) AS st_astext, \
                                name AS name \
                                FROM \
                                %1 \
                                WHERE \
                                (%3 @> '\"highway\"=>\"bus_stop\"' OR \
                                %3 @> '\"amenity\"=>\"bus_station\"') AND \
                                ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) ")
                        .arg( app->property("postgis-polygons").toString() )
                        .arg( app->property("postgis-polygons_geom").toString() )
                        .arg( app->property("postgis-polygons_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try{
                GSSGeometry* geom = environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() );

                // Create stop
                BusStop* stop = dynamic_cast<BusStop*>( environment->getNetworkEnvironment()->getNodeFromGraph( geom->getRepresentativeCoordinate() , BusStop::staticMetaObject.className() ) );
                if( !stop ){
                    stop = new BusStop( geom );
                    stop->setObjectName( record.value("name").toString() );
                    environment->addAgent( stop );
                }

            } catch(...){}
        }
    }


    // LOAD TRAIN STOPS FROM DATABASE
    Debugger::info( 0 , Q_FUNC_INFO , "Creating train stops");

    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( %2 ) AS st_astext, \
                                name AS name \
                                FROM \
                                %1 \
                                WHERE \
                                (%3 @> '\"railway\"=>\"station\"' OR \
                                %3 @> '\"railway\"=>\"halt\"') AND \
                                ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) ")
                        .arg( app->property("postgis-points").toString() )
                        .arg( app->property("postgis-points_geom").toString() )
                        .arg( app->property("postgis-points_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try{
                GSSGeometry* geom = environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() );

                // Create stop
                RailwayStop* stop = dynamic_cast<RailwayStop*>( environment->getNetworkEnvironment()->getNodeFromGraph( geom->getRepresentativeCoordinate() , RailwayStop::staticMetaObject.className() ) );
                if( !stop ){
                    stop = new RailwayStop( geom );
                    stop->setObjectName( record.value("name").toString() );
                    environment->addAgent( stop );
                }

            } catch(...){}
        }
    }

    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( %2 ) AS st_astext, \
                                name AS name \
                                FROM \
                                %1 \
                                WHERE \
                                (%3 @> '\"railway\"=>\"station\"' OR \
                                %3 @> '\"railway\"=>\"halt\"') AND \
                                ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) ")
                        .arg( app->property("postgis-polygons").toString() )
                        .arg( app->property("postgis-polygons_geom").toString() )
                        .arg( app->property("postgis-polygons_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try{
                GSSGeometry* geom = environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() );

                // Create stop
                RailwayStop* stop = dynamic_cast<RailwayStop*>( environment->getNetworkEnvironment()->getNodeFromGraph( geom->getRepresentativeCoordinate() , RailwayStop::staticMetaObject.className() ) );
                if( !stop ){
                    stop = new RailwayStop( geom );
                    stop->setObjectName( record.value("name").toString() );
                    environment->addAgent( stop );
                }

            } catch(...){}
        }
    }


    // LOAD ROADS FROM DATABASE
    Debugger::info( 0 , Q_FUNC_INFO , "Creating roads");


    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( ST_MakeLine(sp,ep) ) AS st_astext, \
                                ST_Length( ST_MakeLine(sp,ep)::geography ) AS st_length, \
                                key AS key, \
                                value AS value, \
                                speed AS speed, \
                                oneway AS oneway, \
                                junction AS junction, \
                                lanes AS lanes \
                                FROM    \
                                ( \
                                    SELECT \
                                    ST_pointn( segmentized.geom , generate_series(1, ST_npoints( segmentized.geom ) -1 )) AS sp, \
                                    ST_pointn( segmentized.geom , generate_series(2, ST_npoints( segmentized.geom )  )) AS ep, \
                                    'highway' AS key, \
                                    tags->'highway' AS value, \
                                    tags->'speed' AS speed, \
                                    tags->'oneway' AS oneway, \
                                    tags->'junction' AS junction, \
                                    tags->'lanes' AS lanes \
                                    FROM \
                                    ( \
                                        SELECT ST_SEGMENTIZE( %2 , 0.0001 ) AS geom , %3 AS tags \
                                        FROM %1 \
                                        WHERE ( \
                                                (%3->'highway') IS NOT NULL OR \
                                                (%3->'junction') IS NOT NULL ) AND \
                                        ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) \
                                    ) AS segmentized \
                                    \
                                ) AS foo;")
                        .arg( app->property("postgis-lines").toString() )
                        .arg( app->property("postgis-lines_geom").toString() )
                        .arg( app->property("postgis-lines_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try{
                RoadConstants::road_types road_type = RoadConstants::fromOSM( record.value("key").toString() , record.value("value").toString() );

                if( road_type == RoadConstants::UNCLASSIFIED ){
                    continue;
                }

                GSSLineString* line = dynamic_cast<GSSLineString*>( environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() ) );

                TransportNode* start = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getFirstCoordinate() , BusStop::staticMetaObject.className() ) );
                if( !start ){
                    start = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getFirstCoordinate() , Road::staticMetaObject.className() ) );
                }
                if( !start ){
                    start = new TransportNode( line->getFirstCoordinate() );
                }

                TransportNode* end = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getLastCoordinate() , BusStop::staticMetaObject.className() ) );
                if( !end ){
                    end = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getLastCoordinate() , Road::staticMetaObject.className() ) );
                }
                if( !end ){
                    end = new TransportNode( line->getLastCoordinate() );
                }

                // One direction
                {
                    Road* road = new Road( road_type , start , end , dynamic_cast<GSSLineString*>( line->createCopy() ) );
                    road->setLength( LengthUnit( record.value("st_length").toDouble() ) ); // In meters

                    int lanes = qMax( 1 , record.value("lanes").toInt() );
                    LengthUnit car_length = LengthUnit(10);
                    road->setMaxCapacity( lanes * qMax( 1.0 , road->getLength().number() / car_length.number() ) );

                    environment->addAgent( road );
                }

                // Reverse direction
                if ( !record.value("oneway").toBool() && record.value("junction").toString() != "roundabout" ){

                    Road* road = new Road( road_type , end , start , line->createReversed() );
                    road->setLength( LengthUnit( record.value("st_length").toDouble() ) ); // In meters

                    int lanes = qMax( 1 , record.value("lanes").toInt() );
                    LengthUnit car_length = LengthUnit(10);
                    road->setMaxCapacity( lanes * qMax( 1.0 , road->getLength().number() / car_length.number() ) );

                    environment->addAgent( road );
                }
                line->deleteLater();

            } catch(...){}
        }
    }

    // LOAD RAILWAYS FROM DATABASE
    Debugger::info( 0 , Q_FUNC_INFO , "Creating railways");

    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( ST_MakeLine(sp,ep) ) AS st_astext, \
                                ST_Length( ST_MakeLine(sp,ep)::geography ) AS st_length, \
                                key AS key, \
                                value AS value, \
                                speed AS speed, \
                                oneway AS oneway, \
                                lanes AS lanes \
                                FROM    \
                                ( \
                                    SELECT \
                                    ST_pointn( segmentized.geom , generate_series(1, ST_npoints( segmentized.geom ) -1 )) AS sp, \
                                    ST_pointn( segmentized.geom , generate_series(2, ST_npoints( segmentized.geom )  )) AS ep, \
                                    'railway' AS key, \
                                    tags->'railway' AS value, \
                                    tags->'speed' AS speed, \
                                    tags->'oneway' AS oneway, \
                                    tags->'lanes' AS lanes \
                                    FROM \
                                    ( \
                                        SELECT ST_SEGMENTIZE( %2 , 1 ) AS geom , %3 AS tags \
                                        FROM %1 \
                                        WHERE ( (%3->'railway') IS NOT NULL ) AND \
                                        ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) \
                                    ) AS segmentized \
                                    \
                                ) AS foo;")
                        .arg( app->property("postgis-lines").toString() )
                        .arg( app->property("postgis-lines_geom").toString() )
                        .arg( app->property("postgis-lines_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try{
                RailwayConstants::railway_types railway_type = RailwayConstants::fromOSM( record.value("key").toString() , record.value("value").toString() );

                if( railway_type == RailwayConstants::UNCLASSIFIED ){
                    continue;
                }

                GSSLineString* line = dynamic_cast<GSSLineString*>( environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() ) );

                TransportNode* start = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getFirstCoordinate() , RailwayStop::staticMetaObject.className() ) );
                if( !start ){
                    start = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getFirstCoordinate() , Railway::staticMetaObject.className() ) );
                }
                if( !start ){
                    start = new TransportNode( line->getFirstCoordinate() );
                }

                TransportNode* end = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getLastCoordinate() , RailwayStop::staticMetaObject.className() ) );
                if( !end ){
                    end = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getLastCoordinate() , Railway::staticMetaObject.className() ) );
                }
                if( !end ){
                    end = new TransportNode( line->getLastCoordinate() );
                }

                // One direction
                {
                    Railway* railway = new Railway( railway_type , start , end , dynamic_cast<GSSLineString*>( line->createCopy() ) );
                    railway->setLength( LengthUnit( record.value("st_length").toDouble() ) ); // In meters
                    railway->setMaxCapacity( 1 );
                    environment->addAgent( railway );
                }

                // Reverse direction
                {
                    Railway* railway = new Railway( railway_type , end , start , line->createReversed() );
                    railway->setLength( LengthUnit( record.value("st_length").toDouble() ) ); // In meters
                    railway->setMaxCapacity( 1 );
                    environment->addAgent( railway );
                }
                line->deleteLater();

            } catch(...){}
        }
    }

    // LOAD WALK PATHS FROM DATABASE
    Debugger::info( 0 , Q_FUNC_INFO , "Creating walk paths");

    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( ST_MakeLine(sp,ep) ) AS st_astext, \
                                ST_Length( ST_MakeLine(sp,ep)::geography ) AS st_length, \
                                key AS key, \
                                value AS value, \
                                speed AS speed, \
                                oneway AS oneway, \
                                lanes AS lanes \
                                FROM    \
                                ( \
                                    SELECT \
                                    ST_pointn( segmentized.geom , generate_series(1, ST_npoints( segmentized.geom ) -1 )) AS sp, \
                                    ST_pointn( segmentized.geom , generate_series(2, ST_npoints( segmentized.geom )  )) AS ep, \
                                    'highway' AS key, \
                                    tags->'highway' AS value, \
                                    tags->'speed' AS speed, \
                                    tags->'oneway' AS oneway, \
                                    tags->'lanes' AS lanes \
                                    FROM \
                                    ( \
                                        SELECT ST_SEGMENTIZE( %2 , 0.001 ) AS geom , %3 AS tags \
                                        FROM %1 \
                                        WHERE ( (%3->'highway') IS NOT NULL \
                                        OR (%3->'junction') IS NOT NULL ) AND \
                                        ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) \
                                    ) AS segmentized \
                                    \
                                ) AS foo;")
                        .arg( app->property("postgis-lines").toString() )
                        .arg( app->property("postgis-lines_geom").toString() )
                        .arg( app->property("postgis-lines_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try{
                WalkPathConstants::walk_path_types walk_path_type = WalkPathConstants::fromOSM( record.value("key").toString() , record.value("value").toString() );

                if( walk_path_type == WalkPathConstants::UNCLASSIFIED ){
                    continue;
                }

                GSSLineString* line = dynamic_cast<GSSLineString*>( environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() ) );

                TransportNode* start = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getFirstCoordinate() , BusStop::staticMetaObject.className() ) );
                if( !start ){
                    start = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getFirstCoordinate() , WalkPath::staticMetaObject.className() ) );
                }
                if( !start ){
                    start = new TransportNode( line->getFirstCoordinate() );
                }

                TransportNode* end = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getLastCoordinate() , BusStop::staticMetaObject.className() ) );
                if( !end ){
                    end = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( line->getLastCoordinate() , WalkPath::staticMetaObject.className() ) );
                }
                if( !end ){
                    end = new TransportNode( line->getLastCoordinate() );
                }

                // One direction
                {
                    WalkPath* road = new WalkPath( walk_path_type , start , end , dynamic_cast<GSSLineString*>( line->createCopy() ) );
                    road->setLength( LengthUnit( record.value("st_length").toDouble() ) ); // In meters
                    road->setMaxCapacity( 1000 );
                    environment->addAgent( road );
                }

                // Reverse direction
                {
                    WalkPath* road = new WalkPath( walk_path_type , end , start , line->createReversed() );
                    road->setLength( LengthUnit( record.value("st_length").toDouble() ) ); // In meters
                    road->setMaxCapacity( 1000 );
                    environment->addAgent( road );
                }
                line->deleteLater();

            } catch(...){}
        }
    }

    // LOAD PARKINGS FROM DATABASE
    Debugger::info( 0 , Q_FUNC_INFO , "Creating parkings");

    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( %2 ) AS st_astext, \
                                name AS name \
                                FROM \
                                %1 \
                                WHERE \
                                (%3 @> '\"amenity\"=>\"parking\"' ) AND \
                                ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) ")
                        .arg( app->property("postgis-polygons").toString() )
                        .arg( app->property("postgis-polygons_geom").toString() )
                        .arg( app->property("postgis-polygons_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try {
                GSSGeometry* geom = environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() );

                // Create parking
                Parking* parking = new Parking( geom );
                environment->addAgent( parking );

            } catch(...){}
        }
    }

    // LOAD CHARGING STATIONS FROM DATABASE
    Debugger::info( 0 , Q_FUNC_INFO , "Creating charging stations");

    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( %2 ) AS st_astext, \
                                name AS name \
                                FROM \
                                %1 \
                                WHERE \
                                (%3 @> '\"amenity\"=>\"charging_station\"' ) AND \
                                ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) ")
                        .arg( app->property("postgis-points").toString() )
                        .arg( app->property("postgis-points_geom").toString() )
                        .arg( app->property("postgis-points_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try {
                GSSGeometry* geom = environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() );

                // Create charging_station
                ChargingStation* station = new ChargingStation( geom );
                environment->addAgent( station );

            } catch(...){}
        }

        // Lets add also one to every parking
        foreach(Agent* a , environment->getByClass( Parking::staticMetaObject.className() )){
            GSSGeometry* geom = environment->getPhysicalEnvironment()->getGeometryController()->createPoint( a->getRepresentativeCoordinate() );
            ChargingStation* station = new ChargingStation( geom );
            environment->addAgent( station );
        }
    }

    // CONNECT BUS STOPS TO ROAD AND WALKPATHS
    Debugger::info( 0 , Q_FUNC_INFO , "Connecting bus stops");

    {
        foreach(Agent* a , environment->getByClass( BusStop::staticMetaObject.className() )){

            BusStop* connect = dynamic_cast<BusStop*>( a );

            // Connect the stop to its nearest road
            Road* near_road = dynamic_cast<Road*>( environment->getPhysicalEnvironment()->getNearestAgent( connect->getRepresentativeCoordinate() , Road::staticMetaObject.className() ) );
            if( near_road ){

                if( near_road && near_road->getToNode()->getCoordinate() != connect->getCoordinate() ){

                    {
                        Road* road = new Road( near_road->getRoadType() , connect , near_road->getToNode() , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( connect->getRepresentativeCoordinate() , near_road->getToNode()->getCoordinate() ) );
                        environment->addAgent( road );
                    }

                    {
                        Road* road = new Road( near_road->getRoadType() , near_road->getToNode() , connect , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( near_road->getToNode()->getCoordinate() , connect->getRepresentativeCoordinate() ) );
                        environment->addAgent( road );
                    }
                }
            }

            // Connect stop to its nearest WalkPath
            WalkPath* near_walkpath = dynamic_cast<WalkPath*>( environment->getPhysicalEnvironment()->getNearestAgent( connect->getRepresentativeCoordinate() , WalkPath::staticMetaObject.className() ) );
            if( near_walkpath ){

                if( near_walkpath && near_walkpath->getToNode()->getCoordinate() != connect->getCoordinate() ){

                    {
                        WalkPath* walk = new WalkPath( near_walkpath->getWalkPathType() , connect , near_walkpath->getToNode() , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( connect->getRepresentativeCoordinate() , near_walkpath->getToNode()->getCoordinate() ) );
                        environment->addAgent( walk );
                    }

                    {
                        WalkPath* walk = new WalkPath( near_walkpath->getWalkPathType() , near_walkpath->getToNode() , connect , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( near_walkpath->getToNode()->getCoordinate() , connect->getRepresentativeCoordinate() ) );
                        environment->addAgent( walk );
                    }
                }
            }
        }
    }


    // CONNECT PARKINGS TO ROAD AND WALKPATHS
    Debugger::info( 0 , Q_FUNC_INFO , "Connecting parkings");
    {
        foreach(Agent* a , environment->getByClass( Parking::staticMetaObject.className() )){

            Parking* connect = dynamic_cast<Parking*>( a );

            // Connect the stop to its nearest road
            Road* near_road = dynamic_cast<Road*>( environment->getPhysicalEnvironment()->getNearestAgent( connect->getRepresentativeCoordinate() , Road::staticMetaObject.className() ) );
            if( near_road ){

                if( near_road && near_road->getToNode()->getCoordinate() != connect->getCoordinate() ){

                    {
                        Road* road = new Road( near_road->getRoadType() , connect , near_road->getToNode() , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( connect->getRepresentativeCoordinate() , near_road->getToNode()->getCoordinate() ) );
                        environment->addAgent( road );
                    }

                    {
                        Road* road = new Road( near_road->getRoadType() , near_road->getToNode() , connect , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( near_road->getToNode()->getCoordinate() , connect->getRepresentativeCoordinate() ) );
                        environment->addAgent( road );
                    }
                }
            }

            // Connect stop to its nearest WalkPath
            WalkPath* near_walkpath = dynamic_cast<WalkPath*>( environment->getPhysicalEnvironment()->getNearestAgent( connect->getRepresentativeCoordinate() , WalkPath::staticMetaObject.className() ) );
            if( near_walkpath ){

                if( near_walkpath && near_walkpath->getToNode()->getCoordinate() != connect->getCoordinate() ){

                    {
                        WalkPath* walk = new WalkPath( near_walkpath->getWalkPathType() , connect , near_walkpath->getToNode() , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( connect->getRepresentativeCoordinate() , near_walkpath->getToNode()->getCoordinate() ) );
                        environment->addAgent( walk );
                    }

                    {
                        WalkPath* walk = new WalkPath( near_walkpath->getWalkPathType() , near_walkpath->getToNode() , connect , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( near_walkpath->getToNode()->getCoordinate() , connect->getRepresentativeCoordinate() ) );
                        environment->addAgent( walk );
                    }
                }
            }
        }
    }


    // CONNECT CHARGING STATIONS TO ROAD AND WALKPATHS
    Debugger::info( 0 , Q_FUNC_INFO , "Connecting charging stations");
    {
        foreach(Agent* a , environment->getByClass( ChargingStation::staticMetaObject.className() )){

            ChargingStation* connect = dynamic_cast<ChargingStation*>( a );

            // Connect the stop to its nearest road
            Road* near_road = dynamic_cast<Road*>( environment->getPhysicalEnvironment()->getNearestAgent( connect->getRepresentativeCoordinate() , Road::staticMetaObject.className() ) );
            if( near_road ){

                if( near_road && near_road->getToNode()->getCoordinate() != connect->getCoordinate() ){

                    {
                        Road* road = new Road( near_road->getRoadType() , connect , near_road->getToNode() , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( connect->getRepresentativeCoordinate() , near_road->getToNode()->getCoordinate() ) );
                        environment->addAgent( road );
                    }

                    {
                        Road* road = new Road( near_road->getRoadType() , near_road->getToNode() , connect , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( near_road->getToNode()->getCoordinate() , connect->getRepresentativeCoordinate() ) );
                        environment->addAgent( road );
                    }
                }
            }

            // Connect stop to its nearest WalkPath
            WalkPath* near_walkpath = dynamic_cast<WalkPath*>( environment->getPhysicalEnvironment()->getNearestAgent( connect->getRepresentativeCoordinate() , WalkPath::staticMetaObject.className() ) );
            if( near_walkpath ){

                if( near_walkpath && near_walkpath->getToNode()->getCoordinate() != connect->getCoordinate() ){

                    {
                        WalkPath* walk = new WalkPath( near_walkpath->getWalkPathType() , connect , near_walkpath->getToNode() , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( connect->getRepresentativeCoordinate() , near_walkpath->getToNode()->getCoordinate() ) );
                        environment->addAgent( walk );
                    }

                    {
                        WalkPath* walk = new WalkPath( near_walkpath->getWalkPathType() , near_walkpath->getToNode() , connect , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( near_walkpath->getToNode()->getCoordinate() , connect->getRepresentativeCoordinate() ) );
                        environment->addAgent( walk );
                    }
                }
            }
        }
    }


    // CONNECT TRAIN STOPS TO RAILWAY AND WALKPATHS
    Debugger::info( 0 , Q_FUNC_INFO , "Connecting train stops");
    {
        foreach(Agent* a , environment->getByClass( RailwayStop::staticMetaObject.className() )){

            RailwayStop* connect = dynamic_cast<RailwayStop*>( a );

            // Connect the stop to its nearest road
            Railway* near_rail = dynamic_cast<Railway*>( environment->getPhysicalEnvironment()->getNearestAgent( connect->getRepresentativeCoordinate() , Railway::staticMetaObject.className() ) );
            if( near_rail ){

                if( near_rail && near_rail->getToNode()->getCoordinate() != connect->getCoordinate() ){

                    {
                        Railway* road = new Railway( near_rail->getRailwayType() , connect , near_rail->getToNode() , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( connect->getRepresentativeCoordinate() , near_rail->getToNode()->getCoordinate() ) );
                        environment->addAgent( road );
                    }

                    {
                        Railway* road = new Railway( near_rail->getRailwayType() , near_rail->getToNode() , connect , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( near_rail->getToNode()->getCoordinate() , connect->getRepresentativeCoordinate() ) );
                        environment->addAgent( road );
                    }
                }
            }

            // Connect stop to its nearest WalkPath
            WalkPath* near_walkpath = dynamic_cast<WalkPath*>( environment->getPhysicalEnvironment()->getNearestAgent( connect->getRepresentativeCoordinate() , WalkPath::staticMetaObject.className() ) );
            if( near_walkpath ){

                if( near_walkpath && near_walkpath->getToNode()->getCoordinate() != connect->getCoordinate() ){

                    {
                        WalkPath* walk = new WalkPath( near_walkpath->getWalkPathType() , connect , near_walkpath->getToNode() , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( connect->getRepresentativeCoordinate() , near_walkpath->getToNode()->getCoordinate() ) );
                        environment->addAgent( walk );
                    }

                    {
                        WalkPath* walk = new WalkPath( near_walkpath->getWalkPathType() , near_walkpath->getToNode() , connect , environment->getPhysicalEnvironment()->getGeometryController()->createLineString( near_walkpath->getToNode()->getCoordinate() , connect->getRepresentativeCoordinate() ) );
                        environment->addAgent( walk );
                    }
                }
            }
        }
    }


    // LOAD BIKE RENTALS FROM DATABASE
    Debugger::info( 0 , Q_FUNC_INFO , "Creating bike rentals");
    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( %2 ) AS st_astext, \
                                name AS name \
                                FROM \
                                %1 \
                                WHERE \
                                (%3 @> '\"amenity\"=>\"bicycle_rental\"') AND \
                                ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) ")
                        .arg( app->property("postgis-points").toString() )
                        .arg( app->property("postgis-points_geom").toString() )
                        .arg( app->property("postgis-points_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try{
                GSSGeometry* geom = environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() );

                // Create rental
                BicycleRental* rental = new BicycleRental( geom );
                rental->setObjectName( record.value("name").toString() );
                environment->addAgent( rental );

            } catch(...){}
        }
    }

    // LOAD TRAFFIC LIGHTS FROM DATABASE
    Debugger::info( 0 , Q_FUNC_INFO , "Creating traffic lights");
    {
        QListIterator<QSqlRecord> i(
                    db->executeQuery(
                        QString("SELECT \
                                ST_AsText( %2 ) AS st_astext, \
                                name AS name \
                                FROM \
                                %1 \
                                WHERE \
                                %3 @> '\"highway\"=>\"traffic_signals\"' AND \
                                ST_INTERSECTS( %2 , ST_GEOMFROMTEXT('%4' , 4326)) ")
                        .arg( app->property("postgis-points").toString() )
                        .arg( app->property("postgis-points_geom").toString() )
                        .arg( app->property("postgis-points_tags").toString() )
                        .arg( environment_geometry->toString() )));

        while( i.hasNext() ){
            QSqlRecord record = i.next();
            try{
                GSSGeometry* geom = environment->getPhysicalEnvironment()->getGeometryController()->createGeometry( record.value("st_astext").toString() );

                TrafficLight* light = new TrafficLight( geom );
                light->setObjectName( record.value("name").toString() );
                environment->addAgent( light );

                GSSGraphNode* node = environment->getNetworkEnvironment()->getNearestNodeFromGraph( light->getRepresentativeCoordinate() , Road::staticMetaObject.className() );
                if( node ){
                    foreach(GSSGraphEdge* e , node->getArrivingEdges()){
                        if( dynamic_cast<Road*>( e ) ){
                            QList<TransportLine*> roads;
                            roads.append( dynamic_cast<Road*>( e ) );
                            light->addRoadGroup( TimeUnit( qrand() % 20 + 10 ) , roads );
                        }
                    }
                }

            } catch(...){}
        }
    }

    db->deleteLater();




    // LOAD GTFS FROM SCENARIO FOLDER
    Debugger::info( 0 , Q_FUNC_INFO , "Creating gtfs");
    {
        QDir gtfs_folder("./../usecases/gtplat/scenario/gtfs");
        foreach( QString folder_name , gtfs_folder.entryList() ){

            QDir folder( gtfs_folder.absoluteFilePath( folder_name ) );
            if ( !folder.exists() || folder_name == "." || folder_name == ".." ) {
                continue;
            }

            GtfsImporter* gtfs = new GtfsImporter( gtfs_folder.absoluteFilePath( folder_name ) );
            gtfs->createStretchsInEnvironment( environment , environment->getTimeEnvironment()->getCurrentDateTime() , environment->getTimeEnvironment()->getCurrentDateTime().addDays( 1 ) );

            foreach(PublicTransportVehicleDriver* driver , gtfs->getGTFSDrivers( environment , environment->getTimeEnvironment()->getCurrentDateTime() , environment->getTimeEnvironment()->getCurrentDateTime().addDays( 1 ) )){

                if( !driver->getItinerary().isEmpty() ){

                    PublicTransportVehicle* vehicle = 0;
                    QString graph_name = TransportLine::staticMetaObject.className();
                    if( driver->getRouteVehicleType() == Tram::staticMetaObject.className() ){
                        vehicle = new Tram( driver->getGeometry()->createCopy() , driver );
                        graph_name = Railway::staticMetaObject.className(); }
                    if( driver->getRouteVehicleType() == Metro::staticMetaObject.className() ){
                        vehicle = new Metro( driver->getGeometry()->createCopy() , driver );
                        graph_name = Railway::staticMetaObject.className(); }
                    if( driver->getRouteVehicleType() == Train::staticMetaObject.className() ){
                        vehicle = new Train( driver->getGeometry()->createCopy() , driver );
                        graph_name = Railway::staticMetaObject.className(); }
                    if( driver->getRouteVehicleType() == Bus::staticMetaObject.className() ){
                        vehicle = new Bus( driver->getGeometry()->createCopy() , driver );
                        graph_name = Road::staticMetaObject.className(); }
                    if( driver->getRouteVehicleType() == Ferry::staticMetaObject.className() ){
                        vehicle = new Ferry( driver->getGeometry()->createCopy() , driver );
                        graph_name = Railway::staticMetaObject.className(); }
                    if( driver->getRouteVehicleType() == Funicular::staticMetaObject.className() ){
                        vehicle = new Funicular( driver->getGeometry()->createCopy() , driver );
                        graph_name = Railway::staticMetaObject.className(); }

                    driver->setVehicle( vehicle );
                    vehicle->setObjectName( driver->getRouteId() + " - " + driver->getRouteName() );
                    vehicle->setGeometry( driver->getGeometry()->createCopy() );

                    // Create its TransportLineRoutingSkill according to its vehicle type
                    foreach(Skill* s , driver->getSkills( TransportLineRoutingSkill::staticMetaObject.className() ) ){
                        driver->removeSkill( s );
                        s->deleteLater();
                    }
                    driver->addSkill( new TransportLineRoutingSkill( vehicle , driver , graph_name ) );

                    // Add to environment
                    environment->addAgent( driver );
                    environment->addAgent( vehicle );
                } else {
                    delete driver;
                }
            }
            gtfs->deleteLater();
        }
    }

    // CREATE OPTIONAL HTTP SERVER THAT CAN ACCESS ENVIRONMENT
    new HttpServer( app , environment , 3008 );

    // CREATE OPTIONAL WS SERVER THAT CAN ACCESS ENVIRONMENT
    WebSocketServer* ws_server = new WebSocketServer( app , environment , 3009 );

    // CREATE OPTIONAL TILE SERVER THAT CAN ACCESS ENVIRONMENT
    new TileServer( app , environment , 3010 );

    // CREATE GTPLATSCENARIOCREATOR
    GTPlatScenarioCreator* gtplat_scenario_creator = new GTPlatScenarioCreator( environment , ws_server );
    gtplat_scenario_creator->loadScenario();

    Debugger::info( 0 , Q_FUNC_INFO , "GTPLAT SIMULATOR READY");

    // Finished log
    Logger::log(
                QString("%1;%2;%3;%4;%5;%6;%7;%8;%9;%10;%11;%12;%13;%14;%15;%16")
                .arg( "ID" )
                .arg( "FROM" )
                .arg( "TO" )
                .arg( "PROFILE OPTIONS" )
                .arg( "CHOSEN" )
                .arg( "DEPARTURE" )
                .arg( "EXPECTED DURATION" )
                .arg( "EXPECTED LENGTH" )
                .arg( "EXPECTED PRICE" )
                .arg( "EXPECTED CC" )
                .arg( "EXPECTED DALY" )
                .arg( "REAL DURATION" )
                .arg( "REAL LENGTH" )
                .arg( "REAL PRICE" )
                .arg( "REAL CC" )
                .arg( "REAL DALY" )
                , "FINISHED" );

    // LAUNCH CHROME
    QProcess process;
    process.start("chromium-browser", QStringList() << "--app=http://localhost:3008");

    //QTimer::singleShot( 2000 , environment , &Environment::runEnvironment );

    // START APPLICATION
    exec_forever( app );

}
