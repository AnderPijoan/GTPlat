
#include <QDebug>
#include <QtMath>
#include <QMap>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>

#include "GTPlatProfile.h"
#include "GTPlatJourney.h"
#include "GTPlatItinerary.h"

#include "usecases/gtplat/GTPlatScenarioCreator.h"
#include "usecases/gtplat/GTPlatAgent.h"
#include "usecases/gtplat/GTPlatFuzzyLogicTrainer.h"

#include "utils/exporters/debugger/Debugger.h"
#include "agents/places/constructions/accommodations/Accommodation.h"
#include "agents/places/administrative_areas/AdministrativeArea.h"

#include "utils/geometry/GSSGeometryController.h"

#include "utils/exporters/logger/Logger.h"
#include "utils/exporters/debugger/Debugger.h"
#include "utils/geometry/GSSGeometry.h"
#include "utils/db/PostgresDriver.h"
#include "utils/random_generators/RouletteWheel.h"
#include "utils/api/SyncAPIDriver.h"

int main(int argc, char *argv[]){

    // CREATE QAPPLICATION
    QCoreApplication* app = new QCoreApplication( argc , argv );
    app->thread()->setPriority( QThread::TimeCriticalPriority );

    Environment* environment = new Environment( app );
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

    // LOAD PROFILES FROM SCENARO FOLDER
    QMap<QString,GTPlatProfile*> profiles;
    Debugger::info( 0 , Q_FUNC_INFO , "Reading profiles");
    {
        QDir folder("./../usecases/gtplat/scenario/profiles");
        foreach( QString file_name , folder.entryList() ){

            QFile file( folder.absoluteFilePath( file_name ) );

            if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {

                QJsonParseError jerror;
                QJsonDocument json_document = QJsonDocument::fromJson( file.readAll() , &jerror);

                if(jerror.error != QJsonParseError::NoError){
                    Debugger::warning( 0 , Q_FUNC_INFO , QString("Error when parsing profiles JSON: %1").arg( jerror.errorString() ) );
                    continue;
                }

                QJsonArray pps = json_document.array();

                for(int i = 0; i < pps.size(); i++){
                    QJsonObject p = pps.at(i).toObject();
                    GTPlatProfile* profile = new GTPlatProfile( p );
                    profiles.insert( profile->objectName() , profile );
                    Debugger::info( 0 , Q_FUNC_INFO , QString("Created profile %1").arg( profile->objectName() ) );
                }

                file.close();
            }
        }
    }

    // LOAD JOURNEYS FROM SCENARO FOLDER
    QList<GTPlatJourney*> journeys;
    Debugger::info( 0 , Q_FUNC_INFO , "Reading journeys");
    {
        QDir folder("./../usecases/gtplat/scenario/fl_trainning");
        foreach( QString file_name , folder.entryList() ){

            QFile file( folder.absoluteFilePath( file_name ) );

            if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {

                QJsonParseError jerror;
                QJsonDocument json_document = QJsonDocument::fromJson( file.readAll() , &jerror);

                if(jerror.error != QJsonParseError::NoError){
                    Debugger::warning( 0 , Q_FUNC_INFO , QString("Error when parsing profiles JSON: %1").arg( jerror.errorString() ) );
                    continue;
                }

                QJsonArray jjs = json_document.array();

                for(int i = 0; i < jjs.size(); i++){

                    QJsonObject j = jjs.at(i).toObject();
                    GTPlatJourney* journey = new GTPlatJourney();
                    journey->fromJSON( j );
                    journey->profile = profiles.value( j.value( "profile" ).toString() );
                    if( !journey->profile ){
                        Debugger::critical( 0 , Q_FUNC_INFO , QString("Error trying to create journey for profile %1").arg( j.value("profile").toString() ) );
                    }
                    journey->from = dynamic_cast<AdministrativeArea*>( environment->getByClassAndName( AdministrativeArea::staticMetaObject.className() , j.value("from").toString() ) );
                    if( !journey->from ){
                        Debugger::critical( 0 , Q_FUNC_INFO , QString("Error journey boundary %1 not found").arg( j.value("from").toString() ) );
                    }
                    journey->to = dynamic_cast<AdministrativeArea*>( environment->getByClassAndName( AdministrativeArea::staticMetaObject.className() , j.value("to").toString() ) );
                    if( !journey->from ){
                        Debugger::critical( 0 , Q_FUNC_INFO , QString("Error journey boundary %1 not found").arg( j.value("to").toString() ) );
                    }
                    journeys.append( journey );
                }

                file.close();
            }
        }
    }

    // CREATE ROULETTES
    QMap<QString , RouletteWheel*> roulettes;
    Debugger::info( 0 , Q_FUNC_INFO , "Creating random start-end locations generator");
    foreach(Agent* a , environment->getByClass( AdministrativeArea::staticMetaObject.className() )){
        roulettes.insert( a->objectName() , new RouletteWheel() );

        foreach(Agent* b , environment->getPhysicalEnvironment()->getAgentsIntersecting( a->getGeometry() , Accommodation::staticMetaObject.className() ) ) {

            Accommodation* building = dynamic_cast<Accommodation*>( b );
            roulettes[ a->objectName() ]->addElement( building , building->getLevels() * building->getArea().number() );
        }
    }

    // API DRIVER
    SyncAPIDriver* api = new SyncAPIDriver();

    // FUZZY LOGIC VALUES
    QStringList outputs;
    outputs << "WALK" << "BICYCLE" << "TRANSIT" << "MOTORCYCLE" << "CAR";

    QStringList values;
    values << "VERY_LOW" << "LOW" << "MEDIUM" << "HIGH" << "VERY_HIGH";

    QMap<QString , QStringList> inputs;
    foreach( QString output , outputs ){
        inputs.insert( output+"_PRICE" , values );
        inputs.insert( output+"_DISTANCE" , values );
        inputs.insert( output+"_TIME" , values );
        inputs.insert( output+"_EMISSIONS" , values );
    }

    // CREATE ITINERARIES
    QList<GTPlatAgent*> agents;
    foreach(GTPlatJourney* journey, journeys){

        Debugger::info( 0 , Q_FUNC_INFO , QString( "Creating %1 profiles %2 with options : %3").arg( journey->amount * journey->multiply_by ).arg( journey->profile->objectName() ).arg( journey->profile->options.join(',') ) );

        for(int i = 0; i < journey->amount * journey->multiply_by ; i++){

            srand( i );
            QDateTime departure;
            departure.setTime( journey->time );
            departure.setDate( journey->date );

            GTPlatAgent* agent = new GTPlatAgent( journey->profile , departure, PersonConstants::MALE );
            departure = departure.addMSecs( agent->getId() * qrand() % qMax( 1 , journey->time_deviation * 1000) );
            agent->setNextTick( departure );
            agent->start_time = departure;
            {
                if( !roulettes.contains( journey->from->objectName() ) ){
                    Debugger::warning( 0 , Q_FUNC_INFO , QString("No boundary found named %1").arg( journey->from->objectName() ) );
                }
                Accommodation* building = dynamic_cast<Accommodation*>( roulettes[ journey->from->objectName() ]->getElement() );
                if( building ){
                    agent->departure_place = building->getRepresentativeCoordinate();
                } else {
                    agent->departure_place = journey->from->getRepresentativeCoordinate();
                }
                agent->setGeometry( environment->getPhysicalEnvironment()->getGeometryController()->createPoint( agent->departure_place ) );
            }

            {
                if( !roulettes.contains( journey->to->objectName() ) ){
                    Debugger::warning( 0 , Q_FUNC_INFO , QString("No boundary found named %1").arg( journey->from->objectName() ) );
                }
                Accommodation* building = dynamic_cast<Accommodation*>( roulettes[ journey->to->objectName() ]->getElement() );
                if( building ){
                    agent->destination_place = building->getRepresentativeCoordinate();
                } else {
                    agent->destination_place = journey->to->getRepresentativeCoordinate();
                }
            }

            foreach( QString option , outputs ){
                QNetworkReply* reply = agent->requestGTALGItinerary( api , agent->departure_place , agent->destination_place , option );
                agent->itineraries.prepend( new GTPlatItinerary( reply->readAll() , 0 ) );
                Debugger::info( 0 , Q_FUNC_INFO , QString("Itinerary %1 %2").arg( agent->itineraries.at(0)->requested_mode ).arg( agent->itineraries.at( 0 )->route_found ) );
                reply->deleteLater();
            }
            agents.append( agent );
        }
    }

    api->deleteLater();

    // TRAIN FUZZY LOGIC
    GTPlatFuzzyLogicTrainer* train = new GTPlatFuzzyLogicTrainer( agents , inputs , outputs );
    train->init();
    train->execute();

    return 0;

}
