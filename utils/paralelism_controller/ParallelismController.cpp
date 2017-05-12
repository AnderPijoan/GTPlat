#include "ParallelismController.h"

#include "utils/exporters/debugger/Debugger.h"

ParallelismController* ParallelismController::getInstance(){
    static ParallelismController instance;
    return &instance;
}

ParallelismController::ParallelismController() : QObject(){
    this->main_thread = this->thread();

    // EDIT : /etc/security/limits.conf
    // echo 1000000 > /proc/sys/fs/file-max
    // echo 100000 > /proc/sys/kernel/threads-max
    // lsof | grep QtCreator

    // Remove one to be left for main thread
    int thread_limit = qMax( QThreadPool::globalInstance()->maxThreadCount()-1 , QThread::idealThreadCount()-1 );
    thread_limit = qMax( 1 , thread_limit );

    for(int i = 0; i < thread_limit; i++){

        // Create thread
        QThread* thread = new QThread();
        this->available_threads.append( thread );
        thread->start();
    }

    Debugger::debug( this , Q_FUNC_INFO , QString("ParallelismController created with %1 thread").arg( this->available_threads.size() ) );
}

ParallelismController::~ParallelismController(){
    qDeleteAll( this->available_threads );
}

/**********************************************************************
 GETTERS
**********************************************************************/

int ParallelismController::getThreadsCount() const{
    return this->available_threads.size();
}

/**********************************************************************
 METHODS
**********************************************************************/

QThread* ParallelismController::getThread(int agent_id){
    return this->available_threads.at( agent_id % this->available_threads.size() );
}

QThread* ParallelismController::liberateThread(QThread *thread){
    Q_UNUSED(thread)
    return this->main_thread;
}
