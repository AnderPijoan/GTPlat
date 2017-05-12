#include <QDebug>
#include <QFile>
#include <QMutexLocker>
#include <QDir>
#include <QJsonDocument>

#include "Logger.h"

#include "agents/Agent.h"
#include "environment/Environment.h"

QString Logger::logs_folder_name = "../logs";


/**********************************************************************
 METHODS
**********************************************************************/

void Logger::log(QVariant to_write, QString filename){

   QFile logfile( QDir::currentPath() + "/" + Logger::logs_folder_name + "/" + filename );

   if( logfile.open( QIODevice::Append ) ){

       QTextStream out(&logfile);
       out << to_write.toString() << "\n";
       logfile.close();

   } else {

       bool created = false;

       // Check if folder exists
       if( !QDir( QDir::currentPath() + "/" + Logger::logs_folder_name ).exists() ){
           QDir().mkdir( QDir::currentPath() + "/" + Logger::logs_folder_name );
           created = true;
       }

       // Check if file exists
       if( !logfile.exists() ){
           logfile.open( QIODevice::WriteOnly );
           logfile.close();
           created = true;
       }

       // Now try again
       if( created ){ Logger::log( to_write , filename ); }
   }
}

void Logger::log(QStringList list, QString filename){
    foreach(QString s , list){
        Logger::log( s , filename );
    }
}

void Logger::logAgent(Agent *agent, QString filename){

    QString to_write = QString();
    to_write += QString( "%1," ).arg( agent->getEnvironment()->getTimeEnvironment()->getCurrentDateTime().toString( "yyyy-MM-ddThh:mm:ss.z" ) );
    to_write += QString( "%1," ).arg( agent->getId() );
    to_write += QString( "%1," ).arg( agent->metaObject()->className() );
    to_write += QString( "%1," ).arg( agent->objectName() );
    to_write += QString( "%1," ).arg( agent->objectName() );
    to_write += QString( "%1," ).arg( agent->parent() ? agent->parent()->objectName() : "" );

            /*.arg( QDateTime::currentDateTime().toString( "yyyy-MM-ddThh:mm:ss.z" ) )
            .arg( agent->getEnvironment()->getTimeEnvironment()->getCurrentDateTime().toString( "yyyy-MM-ddThh:mm:ss.z" ) )
            .arg( message )
            .arg( agent->getId() )
            .arg( agent->metaObject()->className() )
            .arg( agent->parent() ? agent->parent()->objectName() : "0" )
            .arg( Logger::getDomainPath( *agent->metaObject() ) )
            .arg( QJsonDocument( agent->toJSON() ).toJson().size() )
            .arg( QString( QJsonDocument( agent->toJSON() ).toJson() ).simplified() );*/

    Logger::log( to_write , filename );
}

/**********************************************************************
 PROTECTED
**********************************************************************/

QString Logger::getDomainPath(const QMetaObject agent_class){
    QString domain_path;

    const QMetaObject* obj = &agent_class;
    while( obj ){
        domain_path.prepend( QString("/%1").arg( obj->className() ) );
        obj = obj->superClass();
    }

    return domain_path;
}
