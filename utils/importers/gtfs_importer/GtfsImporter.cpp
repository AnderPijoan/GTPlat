#include "GtfsImporter.h"

#include <QDir>
#include <QFile>
#include <QVector>
#include <QStringList>

#include "environment/Environment.h"
#include "agents/transport_networks/public_transport/PublicTransportStretch.h"

#include "agents/transport_networks/TransportNode.h"
#include "agents/vehicles/public_transport/Bus.h"
#include "agents/vehicles/public_transport/Ferry.h"
#include "agents/vehicles/public_transport/Funicular.h"
#include "agents/vehicles/public_transport/Metro.h"
#include "agents/vehicles/public_transport/Train.h"
#include "agents/vehicles/public_transport/Tram.h"

#include "skills/route/TransportLineRoutingSkill.h"

/**
 * @brief GtfsImporter Give the name of the folder containing the GTFS files.
 * All files must have the standard name.
 * @param path
 * @return
 */
GtfsImporter::GtfsImporter(QString files_folder_path){

    QDir dir(files_folder_path);
    if( !dir.exists() ){
        Debugger::warning( this , Q_FUNC_INFO , QString("Directory %1 doesnt exist").arg( files_folder_path ) );
        return;
    }

    // Load Agency.txt
    {
        QStringList ss;
        ss << "agency_id" << "agency_name" << "agency_phone" << "agency_url" << "agency_timezone" << "agency_lang";
        QFile file( dir.absoluteFilePath("agency.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    GtfsImporter::Agency a;
                    if( positions->contains("agency_id") ){         a.agency_id = values.at( positions->indexOf("agency_id") ); }
                    if( positions->contains("agency_name") ){       a.agency_name = values.at( positions->indexOf("agency_name") ); }
                    if( positions->contains("agency_phone") ){      a.agency_phone = values.at( positions->indexOf("agency_phone") ); }
                    if( positions->contains("agency_url") ){        a.agency_url = values.at( positions->indexOf("agency_url") ); }
                    if( positions->contains("agency_timezone") ){   a.agency_timezone = values.at( positions->indexOf("agency_timezone") ); }
                    if( positions->contains("agency_lang") ){       a.agency_lang = values.at( positions->indexOf("agency_lang") ); }
                    this->agencies.insert( a.agency_id , a );
                    Debugger::info( this , Q_FUNC_INFO , QString("Parsing agency %1").arg( a.agency_name ) );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load Stops.txt
    {
        QStringList ss;
        ss << "stop_id" << "stop_code" << "stop_name" << "stop_desc" << "stop_lat" << "stop_lon" << "zone_id" << "stop_url" << "location_type" << "parent_station" << "stop_timezone" << "wheelchair_boarding";
        QFile file( dir.absoluteFilePath("stops.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    GtfsImporter::Stop s;
                    if( positions->contains("stop_id") ){          s.stop_id = values.at( positions->indexOf("stop_id") ); }
                    if( positions->contains("stop_code") ){        s.stop_code = values.at( positions->indexOf("stop_code") ); }
                    if( positions->contains("stop_name") ){        s.stop_name = values.at( positions->indexOf("stop_name") ); }
                    if( positions->contains("stop_desc") ){        s.stop_desc = values.at( positions->indexOf("stop_desc") ); }
                    if( positions->contains("stop_lat") ){         s.stop_lat = values.at( positions->indexOf("stop_lat") ).toDouble(); }
                    if( positions->contains("stop_lon") ){         s.stop_lon = values.at( positions->indexOf("stop_lon") ).toDouble(); }
                    if( positions->contains("zone_id") ){          s.zone_id = values.at( positions->indexOf("zone_id") ).toInt(); }
                    if( positions->contains("stop_url") ){         s.stop_url = values.at( positions->indexOf("stop_url") ); }
                    if( positions->contains("location_type") ){    s.location_type = values.at( positions->indexOf("location_type") ).toInt(); }
                    if( positions->contains("parent_station") ){   s.parent_station = values.at( positions->indexOf("parent_station") ); }
                    if( positions->contains("stop_timezone") ){    s.stop_timezone = values.at( positions->indexOf("stop_timezone") ); }
                    if( positions->contains("wheelchair_boarding") ){s.wheelchair_boarding = values.at( positions->indexOf("wheelchair_boarding") ).toInt(); }
                    this->stops.insert(s.stop_id , s );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load Routes.txt
    {
        QStringList ss;
        ss << "route_id" << "agency_id" << "route_short_name" << "route_long_name" << "route_desc" << "route_type" << "route_url" << "route_color" << "route_text_color";
        QFile file( dir.absoluteFilePath("routes.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    GtfsImporter::Route r;
                    if( positions->contains("route_id") ){         r.route_id = values.at( positions->indexOf("route_id") ); }
                    if( positions->contains("agency_id") ){        r.agency_id = values.at( positions->indexOf("agency_id") ); }
                    if( positions->contains("route_short_name") ){ r.route_short_name = values.at( positions->indexOf("route_short_name") ); }
                    if( positions->contains("route_long_name") ){  r.route_long_name = values.at( positions->indexOf("route_long_name") ); }
                    if( positions->contains("route_desc") ){       r.route_desc = values.at( positions->indexOf("route_desc") ); }
                    if( positions->contains("route_type") ){       r.route_type = values.at( positions->indexOf("route_type") ).toInt(); }
                    if( positions->contains("route_url") ){        r.route_url = values.at( positions->indexOf("route_url") ); }
                    if( positions->contains("route_color") ){      r.route_color = values.at( positions->indexOf("route_color") ); }
                    if( positions->contains("route_text_color") ){ r.route_text_color = values.at( positions->indexOf("route_text_color") ); }
                    this->routes.insert(r.route_id , r );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load Trips.txt
    {
        QStringList ss;
        ss << "route_id" << "service_id" << "trip_id" << "trip_headsign" << "trip_short_name" << "direction_id" << "block_id" << "shape_id" << "wheelchair_accessible" << "bikes_allowed";
        QFile file( dir.absoluteFilePath("trips.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    GtfsImporter::Trip t;
                    if( positions->contains("route_id") ){         t.route_id = values.at( positions->indexOf("route_id") ); }
                    if( positions->contains("service_id") ){       t.service_id = values.at( positions->indexOf("service_id") ); }
                    if( positions->contains("trip_id") ){          t.trip_id = values.at( positions->indexOf("trip_id") ); }
                    if( positions->contains("trip_headsign") ){    t.trip_headsign = values.at( positions->indexOf("trip_headsign") ); }
                    if( positions->contains("trip_short_name") ){  t.trip_short_name = values.at( positions->indexOf("trip_short_name") ); }
                    if( positions->contains("direction_id") ){     t.direction_id = values.at( positions->indexOf("direction_id") ).toInt(); }
                    if( positions->contains("block_id") ){         t.block_id = values.at( positions->indexOf("block_id") ); }
                    if( positions->contains("shape_id") ){         t.shape_id = values.at( positions->indexOf("shape_id") ); }
                    if( positions->contains("wheelchair_accessible") ){ t.wheelchair_accessible = values.at( positions->indexOf("wheelchair_accessible") ).toInt(); }
                    if( positions->contains("bikes_allowed") ){    t.bikes_allowed = values.at( positions->indexOf("bikes_allowed") ).toInt(); }
                    this->trips.insert(t.trip_id , t );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load Stop_times.txt
    {
        QStringList ss;
        ss << "trip_id" << "arrival_time" << "departure_time" << "stop_id" << "stop_sequence" << "stop_headsign" << "pickup_type" << "drop_off_type" << "shape_dist_traveled" << "timepoint";
        QFile file( dir.absoluteFilePath("stop_times.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    GtfsImporter::StopTime s;
                    if( positions->contains("trip_id") ){          s.trip_id = values.at( positions->indexOf("trip_id") ); }
                    if( positions->contains("arrival_time") ){     s.arrival_time = QTime::fromString( values.at( positions->indexOf("arrival_time") ) , "hh:mm:ss"); }
                    if( positions->contains("departure_time") ){   s.departure_time = QTime::fromString( values.at( positions->indexOf("departure_time") ) , "hh:mm:ss"); }
                    if( positions->contains("stop_id") ){          s.stop_id = values.at( positions->indexOf("stop_id") ); }
                    if( positions->contains("stop_sequence") ){    s.stop_sequence = values.at( positions->indexOf("stop_sequence") ).toInt(); }
                    if( positions->contains("stop_headsign") ){    s.stop_headsign = values.at( positions->indexOf("stop_headsign") ); }
                    if( positions->contains("pickup_type") ){      s.pickup_type = values.at( positions->indexOf("pickup_type") ).toInt(); }
                    if( positions->contains("drop_off_type") ){    s.drop_off_type = values.at( positions->indexOf("drop_off_type") ).toInt(); }
                    if( positions->contains("shape_dist_traveled") ){ s.shape_dist_traveled = values.at( positions->indexOf("shape_dist_traveled") ).toDouble(); }
                    if( positions->contains("timepoint") ){        s.timepoint = values.at( positions->indexOf("timepoint") ).toInt(); }
                    QList<StopTime> ts = this->stoptimes.value(s.trip_id );
                    ts.append( s );
                    this->stoptimes.insert(s.trip_id , ts );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load calendar.txt
    {
        QStringList ss;
        ss << "service_id" << "monday" << "tuesday" << "wednesday" << "thursday" << "friday" << "saturday" << "sunday" << "start_date" << "end_date";
        QFile file( dir.absoluteFilePath("calendar.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    GtfsImporter::Calendar c;
                    if( positions->contains("service_id") ){       c.service_id = values.at( positions->indexOf("service_id") ); }
                    if( positions->contains("monday") ){           c.monday = values.at( positions->indexOf("monday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("tuesday") ){          c.tuesday = values.at( positions->indexOf("tuesday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("wednesday") ){        c.wednesday = values.at( positions->indexOf("wednesday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("thursday") ){         c.thursday = values.at( positions->indexOf("thursday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("friday") ){           c.friday = values.at( positions->indexOf("friday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("saturday") ){         c.saturday = values.at( positions->indexOf("saturday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("sunday") ){           c.sunday = values.at( positions->indexOf("sunday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("start_date") ){       c.start_date = QDate::fromString( values.at( positions->indexOf("start_date") ) , "yyyyMMdd" ); }
                    if( positions->contains("end_date") ){         c.end_date = QDate::fromString( values.at( positions->indexOf("end_date") ) , "yyyyMMdd" ); }
                    this->calendars.insert(c.service_id , c );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load calendar_dates.txt
    {
        QStringList ss;
        ss << "service_id" << "date" << "exception_type";
        QFile file( dir.absoluteFilePath("calendar_dates.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    GtfsImporter::CalendarDate c;
                    if( positions->contains("service_id") ){    c.service_id = values.at( positions->indexOf("service_id") ); }
                    if( positions->contains("date") ){          c.date = QDate::fromString( values.at( positions->indexOf("date") ) , "yyyyMMdd" ); }
                    if( positions->contains("exception_type") ){c.exception_type = values.at( positions->indexOf("exception_type") ).toInt(); }
                    QList<CalendarDate> ts = this->calendardates.value( c.service_id );
                    ts.append( c );
                    this->calendardates.insert( c.service_id , ts );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load shapes.txt
    {
        QStringList ss;
        ss << "shape_id" << "shape_pt_lat" << "shape_pt_lon" << "shape_pt_sequence" << "shape_dist_traveled";
        QFile file( dir.absoluteFilePath("shapes.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    GtfsImporter::Shape c;
                    if( positions->contains("shape_id") ){              c.shape_id = values.at( positions->indexOf("shape_id") ); }
                    if( positions->contains("shape_pt_lat") ){          c.shape_pt_lat = values.at( positions->indexOf("shape_pt_lat") ).toDouble(); }
                    if( positions->contains("shape_pt_lon") ){          c.shape_pt_lon = values.at( positions->indexOf("shape_pt_lon") ).toDouble(); }
                    if( positions->contains("shape_pt_sequence") ){     c.shape_pt_sequence = values.at( positions->indexOf("shape_pt_sequence") ).toDouble(); }
                    if( positions->contains("shape_dist_traveled") ){   c.shape_dist_traveled = values.at( positions->indexOf("shape_dist_traveled") ).toDouble(); }
                    QList<GtfsImporter::Shape> ts = this->shapes.value( c.shape_id );
                    ts.append( c );
                    this->shapes.insert( c.shape_id , ts );
                }
            }
            delete positions;
        }
        file.close();
    }

    Debugger::info( this , Q_FUNC_INFO , QString("Agencies: %1").arg( this->agencies.size() ) );
    Debugger::info( this , Q_FUNC_INFO , QString("Stops: %1").arg( this->stops.size() ) );
    Debugger::info( this , Q_FUNC_INFO , QString("Routes: %1").arg( this->routes.size() ) );
    Debugger::info( this , Q_FUNC_INFO , QString("Trips: %1").arg( this->trips.size() ) );
    Debugger::info( this , Q_FUNC_INFO , QString("StopTimes: %1").arg( this->stoptimes.size() ) );
    Debugger::info( this , Q_FUNC_INFO , QString("Calendars: %1").arg( this->calendars.size() ) );
    Debugger::info( this , Q_FUNC_INFO , QString("CalendarDates: %1").arg( this->calendardates.size() ) );
    Debugger::info( this , Q_FUNC_INFO , QString("Shapes: %1").arg( this->shapes.size() ) );
}

GtfsImporter::~GtfsImporter(){

}

/**********************************************************************
 GETTERS
**********************************************************************/

QList<GtfsImporter::Agency> GtfsImporter::getAgencies(){
    return this->agencies.values();
}

GtfsImporter::Agency GtfsImporter::getAgencyById(QString id){
    return this->agencies.value(id);
}

QList<GtfsImporter::Stop> GtfsImporter::getStops(){
    return this->stops.values();
}

GtfsImporter::Stop GtfsImporter::getStopById(QString id){
    return this->stops.value(id);
}

QList<GtfsImporter::Route> GtfsImporter::getRoutes(){
    return this->routes.values();
}

GtfsImporter::Route GtfsImporter::getRouteById(QString id){
    return this->routes.value(id);
}

QList<GtfsImporter::Trip> GtfsImporter::getTrips(){
    return this->trips.values();
}

GtfsImporter::Trip GtfsImporter::getTripById(QString id){
    return this->trips.value(id);
}

QList<GtfsImporter::StopTime> GtfsImporter::getStopTimes(){
    QList<StopTime> st;
    foreach(QList<StopTime> s , this->stoptimes.values()){
        st.append( s );
    }
    return st;
}

QList<GtfsImporter::StopTime> GtfsImporter::getStopTimesByTripId(QString id){
    return this->stoptimes.value(id);
}

QList<GtfsImporter::Calendar> GtfsImporter::getCalendars(){
    return this->calendars.values();
}

GtfsImporter::Calendar GtfsImporter::getCalendarByServiceId(QString id){
    return this->calendars.value(id);
}

QList<GtfsImporter::CalendarDate> GtfsImporter::getCalendarDates(){
    QList<CalendarDate> st;
    foreach(QList<GtfsImporter::CalendarDate> s , this->calendardates.values()){
        st.append( s );
    }
    return st;
}

QList<GtfsImporter::CalendarDate> GtfsImporter::getCalendarDatesByServiceId(QString id){
    return this->calendardates.value(id);
}

QList<GtfsImporter::Shape> GtfsImporter::getShapes(){
    QList<Shape> st;
    foreach(QList<GtfsImporter::Shape> s , this->shapes.values()){
        st.append( s );
    }
    return st;
}

QList<GtfsImporter::Shape> GtfsImporter::getShapesByShapeId(QString id){
    return this->shapes.value(id);
}

/**********************************************************************
 METHODS
**********************************************************************/

void GtfsImporter::createStretchsInEnvironment( Environment* environment , QDateTime since_limit , QDateTime until_limit ){

    QMap<QString , PublicTransportStop*> snapped_stops; // STOP_ID , PublicTransportStop

    foreach(GtfsImporter::Trip t , this->trips ){

        // Route
        GtfsImporter::Route r = this->getRouteById( t.route_id );
        if( r.route_id.isEmpty() ){ continue; }

        // Store dates for further creation
        QList<QDate> create_dates;

        // Calendar if long time service
        GtfsImporter::Calendar c = this->getCalendarByServiceId( t.service_id );
        if( !c.service_id.isEmpty() ){

            // Create one edge service per day
            for( int day = 0 ; day < qMax( c.start_date , since_limit.date() ).daysTo( qMin( c.end_date , until_limit.date() ) ) ; day++ ){
                QDate date = qMax( c.start_date , since_limit.date() ).addDays( day );

                // Do not create if this trip does not work this day of week
                int weekday = date.dayOfWeek();
                if( weekday == 1 && !c.monday ){ continue; }
                if( weekday == 2 && !c.tuesday ){ continue; }
                if( weekday == 3 && !c.wednesday ){ continue; }
                if( weekday == 4 && !c.thursday ){ continue; }
                if( weekday == 5 && !c.friday ){ continue; }
                if( weekday == 6 && !c.saturday ){ continue; }
                if( weekday == 7 && !c.sunday ){ continue; }

                // Store date
                create_dates.append( date );
            }
        }

        // Or CalendarDate if only certain dates service
        foreach(GtfsImporter::CalendarDate cd , this->getCalendarDatesByServiceId( t.service_id ) ){

            // Check if date is between filter
            if( cd.date < since_limit.date() || cd.date > until_limit.date() ){ continue; }

            // Store date
            create_dates.append( cd.date );
        }

        if( create_dates.isEmpty() ){ continue; }

        // Stop times
        QList<GtfsImporter::StopTime> ordered_stops = this->getOrderedStopTimes( t );

        // Create stretch for each date
        foreach( QDate date , create_dates ){

            // Create Transport stretchs
            for(int i = 0; i < ordered_stops.size()-1; i++){

                GtfsImporter::StopTime st_from = ordered_stops.at( i );

                // Check if stoptime is between filter
                if( QDateTime( date , st_from.departure_time ) < since_limit || QDateTime( date , st_from.departure_time ) > until_limit ){ continue; }

                GtfsImporter::Stop from = this->getStopById( st_from.stop_id );

                // Find the nearest Stops in environment
                PublicTransportStop* from_stop = snapped_stops.value( from.stop_id , 0 );
                if( !from_stop ){
                    from_stop = this->snapToStopInEnvironment( environment , from , this->getRouteVehicleType( r ) );
                    snapped_stops.insert( from.stop_id , from_stop );
                }
                if( !from_stop ){
                    continue;
                }

                // From stop to all following To stops
                for(int j = i+1; j < ordered_stops.size(); j++){

                    GtfsImporter::StopTime st_to = ordered_stops.at( j );

                    // Check if stoptime is between filter
                    if( QDateTime( date , st_to.departure_time ) < since_limit || QDateTime( date , st_to.departure_time ) > until_limit ){ continue; }

                    GtfsImporter::Stop to = this->getStopById( st_to.stop_id );

                    // Find the nearest Stops in environment
                    PublicTransportStop* to_stop = snapped_stops.value( to.stop_id , 0 );
                    if( !to_stop ){
                        to_stop = this->snapToStopInEnvironment( environment , to , this->getRouteVehicleType( r ) );
                        snapped_stops.insert( to.stop_id , to_stop );
                    }
                    if( !to_stop ){
                        continue;
                    }

                    const GSSCoordinate from_coor = from_stop->getRepresentativeCoordinate();
                    const GSSCoordinate to_coor = to_stop->getRepresentativeCoordinate();
                    if( from_coor == to_coor ){
                        continue;
                    }

                    PublicTransportStretch* stretch = 0;
                    const GSSGraphEdge* edge = environment->getNetworkEnvironment()->getEdgeFromGraph( from_coor , to_coor , PublicTransportStretch::staticMetaObject.className() );
                    stretch = const_cast<PublicTransportStretch*>( dynamic_cast<const PublicTransportStretch*>( edge ) );
                    if( !stretch ){

                        TransportNode* from_node = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( from_coor , PublicTransportStretch::staticMetaObject.className() ) );
                        if( !from_node ){
                            from_node = new TransportNode( from_coor );
                        }

                        TransportNode* to_node = dynamic_cast<TransportNode*>( environment->getNetworkEnvironment()->getNodeFromGraph( to_coor , PublicTransportStretch::staticMetaObject.className() ) );
                        if( !to_node ){
                            to_node = new TransportNode( to_coor );
                        }

                        GSSLineString* line = environment->getPhysicalEnvironment()->getGeometryController()->createLineString( from_coor , to_coor );
                        stretch = new PublicTransportStretch( from_stop , to_stop , line );
                        stretch->setObjectName( QString("%1 - %2").arg( from_stop->objectName() ).arg( to_stop->objectName() ) );
                        environment->addAgent( stretch );
                    }

                    stretch->addService( r.route_id , this->getRouteVehicleType( r ) , QDateTime( date, ordered_stops.at(i).departure_time ) , QDateTime( date , ordered_stops.at(j).arrival_time ) , 10 ); // TODO PRICE
                }
            }
        }
    }
}

QList<PublicTransportVehicleDriver*> GtfsImporter::getGTFSDrivers( Environment* environment , QDateTime since_limit , QDateTime until_limit ){

    QList<PublicTransportVehicleDriver*> drivers;
    QMap<QString , PublicTransportStop*> snapped_stops; // STOP_ID , PublicTransportStop

    foreach(GtfsImporter::Trip t , this->trips ){

        // Route
        GtfsImporter::Route r = this->getRouteById( t.route_id );
        if( r.route_id.isEmpty() ){ continue; }

        // Agency
        GtfsImporter::Agency a = this->getAgencyById( r.agency_id );

        // Store dates for after creation
        QList<QDate> create_dates;

        // Calendar if long time service
        GtfsImporter::Calendar c = this->getCalendarByServiceId( t.service_id );
        if( !c.service_id.isEmpty() ){

            // Create one driver per day
            for( int day = 0 ; day < qMax( c.start_date , since_limit.date() ).daysTo( qMin( c.end_date , until_limit.date() ) ) ; day++ ){
                QDate date = qMax( c.start_date , since_limit.date() ).addDays( day );

                // Do not create if this trip does not work this day of week
                int weekday = date.dayOfWeek();
                if( weekday == 1 && !c.monday ){ continue; }
                if( weekday == 2 && !c.tuesday ){ continue; }
                if( weekday == 3 && !c.wednesday ){ continue; }
                if( weekday == 4 && !c.thursday ){ continue; }
                if( weekday == 5 && !c.friday ){ continue; }
                if( weekday == 6 && !c.saturday ){ continue; }
                if( weekday == 7 && !c.sunday ){ continue; }

                // Store date
                create_dates.append( date );
            }
        }

        // Or CalendarDate if only certain dates service
        foreach(GtfsImporter::CalendarDate cd , this->getCalendarDatesByServiceId( t.service_id ) ){

            // Check if date is between filter
            if( cd.date < since_limit.date() || cd.date > until_limit.date() ){ continue; }

            // Store date
            create_dates.append( cd.date );
        }

        if( create_dates.isEmpty() ){ continue; }

        // Stop times come disordered
        QList<GtfsImporter::StopTime> ordered_stops = this->getOrderedStopTimes( t );

        // Create driver for each date
        foreach( QDate date , create_dates ){

            // Create itinerary
            QList<PublicTransportVehicleDriver::ScheduledStop> itinerary;

            // Iterate stops
            foreach(GtfsImporter::StopTime st, ordered_stops){

                // Check if stoptime is between filter
                if( QDateTime( date , st.departure_time ) < since_limit || QDateTime( date , st.departure_time ) > until_limit ){ continue; }

                PublicTransportStop* public_transport_stop = snapped_stops.value( st.stop_id , 0 );
                if( !public_transport_stop ){
                    GtfsImporter::Stop s = this->getStopById( st.stop_id );
                    public_transport_stop = this->snapToStopInEnvironment( environment , s , this->getRouteVehicleType( r ) );
                    snapped_stops.insert( st.stop_id , public_transport_stop );
                }

                PublicTransportVehicleDriver::ScheduledStop scheduled_stop;
                scheduled_stop.stop = public_transport_stop;
                scheduled_stop.arrival = QDateTime( date , st.arrival_time );
                scheduled_stop.departure = QDateTime( date , st.departure_time );
                itinerary.append( scheduled_stop );
            }

            if( itinerary.isEmpty() ){ continue; }

            // Create driver
            PublicTransportVehicleDriver* driver = new PublicTransportVehicleDriver( 0 , PersonConstants::MALE , itinerary.at(0).stop->getGeometry()->createCopy() );
            driver->setItinerary( itinerary );
            driver->setNextTick( itinerary.at(0).arrival );
            driver->setDestination( itinerary.at( itinerary.size()-1 ).stop->getRepresentativeCoordinate() );
            driver->setRouteName( r.route_long_name );
            driver->setRouteId( r.route_id );
            driver->setRouteVehicleType( this->getRouteVehicleType( r ) );
            driver->setAgencyName( a.agency_name );
            driver->setObjectName( QString("%1 - %2 - %3").arg( a.agency_name ).arg( r.route_long_name ).arg( t.trip_headsign ) );

            drivers.append( driver );
        }
    }

    Debugger::info( this , Q_FUNC_INFO , QString("Drivers created: %1").arg( drivers.size() ) );
    return drivers;
}

/**********************************************************************
 PRIVATE METHODS
**********************************************************************/

QString GtfsImporter::getRouteVehicleType(GtfsImporter::Route route){
    switch ( route.route_type ) {
    case 0: return Tram::staticMetaObject.className();
    case 1: return Metro::staticMetaObject.className();
    case 2: return Train::staticMetaObject.className();
    case 3: return Bus::staticMetaObject.className();
    case 4: return Ferry::staticMetaObject.className();
    case 5: return Funicular::staticMetaObject.className();
    case 7: return Funicular::staticMetaObject.className();
    default: return Bus::staticMetaObject.className();
    }
}

QList<GtfsImporter::StopTime> GtfsImporter::getOrderedStopTimes(GtfsImporter::Trip trip){

    QList<GtfsImporter::StopTime> ordered_stop_times;

    QMap<int , GtfsImporter::StopTime> disordered_stop_times;
    foreach(GtfsImporter::StopTime st , this->getStopTimesByTripId( trip.trip_id )){
        disordered_stop_times.insert( st.stop_sequence , st );
    }

    bool ended = false;
    int seq = 0;
    while( !ended ){
        if( disordered_stop_times.keys().contains( ++seq ) ){
            ordered_stop_times.append( disordered_stop_times.value(seq) );
        } else if( disordered_stop_times.keys().contains( ++seq ) ){ // SOME GTFS MAY SKIP ONE STOP
            ordered_stop_times.append( disordered_stop_times.value(seq) );
        } else {
            ended = true;
        }
    }
    return ordered_stop_times;
}

PublicTransportStop* GtfsImporter::snapToStopInEnvironment( Environment* environment , GtfsImporter::Stop stop , QString vehicle_type ){
    PublicTransportStop* public_transport_stop = 0;

    QString stop_type = PublicTransportStop::staticMetaObject.className();

    if( vehicle_type == Tram::staticMetaObject.className() ||
        vehicle_type == Train::staticMetaObject.className() ||
        vehicle_type == Metro::staticMetaObject.className() ||
        vehicle_type == Funicular::staticMetaObject.className() ){
        stop_type = RailwayStop::staticMetaObject.className();
    }
    if( vehicle_type == Bus::staticMetaObject.className() ){
        stop_type = BusStop::staticMetaObject.className();
    }

    public_transport_stop = dynamic_cast<PublicTransportStop*>( environment->getNetworkEnvironment()->getNearestNodeFromGraph( GSSCoordinate(stop.stop_lon , stop.stop_lat) , stop_type ) );

    return public_transport_stop;
}
