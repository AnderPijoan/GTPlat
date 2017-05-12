#include "PerformanceMonitor.h"

#include <QSysInfo>
#include <unistd.h>
#include <sys/resource.h>

#include "utils/paralelism_controller/ParallelismController.h"
#include "utils/exporters/logger/Logger.h"

PerformanceMonitor::PerformanceMonitor(QCoreApplication *app, Environment *environment) : QObject( app ){
    this->environment = environment;
    this->timer = 0;

    foreach(QString s , this->getSystemInfo()){  }
}

/**********************************************************************
 SLOTS
**********************************************************************/

void PerformanceMonitor::startWatchingPerformance(){
    this->max_running_agents_count = 0;
    this->max_threads_count = 0;

    this->elapsed_time.start();

    if( !this->timer ){
        this->timer = new QTimer( this );
    }
    this->timer->setSingleShot( false );
    this->connect( this->timer , SIGNAL(timeout()) , this , SLOT(storeCurrentPerformance()) );
    this->timer->start( 5000 ); // Invoke timeout signal every 5 seconds
}

void PerformanceMonitor::storeCurrentPerformance(){

}

void PerformanceMonitor::stopWatchingPerformance(){
    this->elapsed_time.msecsSinceReference();

    this->timer->stop();
    this->disconnect( this->timer , SIGNAL(timeout()) , this , SLOT(storeCurrentPerformance()) );
    this->timer->deleteLater();
    this->timer = 0;
}

/**********************************************************************
 PRIVATE
**********************************************************************/
QStringList PerformanceMonitor::getSystemInfo(){

    QSysInfo sys;
    QStringList info;
    info << QString("Product - %1").arg( sys.productType() );
    info << QString("Product Version - %1").arg( sys.productVersion() );
    info << QString("Product String - %1").arg( sys.prettyProductName() );
    info << QString("CPU Architecture - %1").arg( sys.currentCpuArchitecture() );
    info << QString("Kernel - %1").arg( sys.kernelType() );
    info << QString("Kernel Version - %1").arg( sys.kernelVersion() );
    return info;

}

QStringList PerformanceMonitor::getUsage(){

    QStringList info;
    // http://linux.die.net/man/2/getrusage

    //rusage st;
    //int s = getrusage( RUSAGE_SELF , &st );
    return info;
}
