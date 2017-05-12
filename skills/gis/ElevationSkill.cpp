#include <QSqlRecord>
#include <QVariant>

#include "ElevationSkill.h"

ElevationSkill::ElevationSkill(QString url, int port, QString db_name, QString db_user, QString db_pass) : Skill(0){
    /*if( agent ){
        this->db_driver = new PostgresDriver( url, port, db_name, db_user, db_pass, agent->objectName() );
    } else {
        this->db_driver = new PostgresDriver( url, port, db_name, db_user, db_pass , "ElevationSkill" );
    }*/
}

ElevationSkill::~ElevationSkill(){
        db_driver->deleteLater();
}

/**********************************************************************
 METHODS
**********************************************************************/

double ElevationSkill::getPointElevation(Point* point){

    // Query database
    QString sql = QString("\
          SELECT AVG(st_value(elevation.rast, point.the_geom)) AS height \
            FROM \
            %1 AS elevation, \
            ( SELECT ST_GEOMFROMTEXT('POINT(%2 %3)' , 4326) AS the_geom ) AS point \
            WHERE ST_INTERSECTS(rast, point.the_geom)")
        .arg("geosmartsim_elevation")   // Table name
        .arg(point->getX())             // point X
        .arg(point->getY());            // point Y

    QListIterator<QSqlRecord> i(this->db_driver->executeQuery( sql ) );

    // Iterate on all the points
    while( i.hasNext() ){
        QSqlRecord record = i.next();
        return record.value("height").toDouble();
    }
    return 0;
}
