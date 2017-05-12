#include "Debugger.h"

#include <QDebug>
#include "agents/Agent.h"
#include "utils/exporters/logger/Logger.h"

int Debugger::debug_show_level = 2 ;
bool Debugger::debug_to_log = false;

Debugger* Debugger::getInstance(){
    static Debugger instance;
    return &instance;
}

Debugger::Debugger() : QObject() {
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Debugger::setDebugLevel(int level){
    Debugger::debug_show_level = level;
}

/**********************************************************************
 METHODS
**********************************************************************/

void Debugger::operation(const QObject *caller, QString function, QString message, int operation){

    const Agent* casted_agent = dynamic_cast<const Agent*>( caller );
    QString caller_string = QString("%1%2")
            .arg( caller ? caller->metaObject()->className() : "" )
            .arg( casted_agent ? QString::number( casted_agent->getId() ) + " " : "" );

    QString caller_and_function = QString("[%1 :: %2]")
            .arg( caller_string )
            .arg( function );

    if( Debugger::debug_show_level <= operation ){

        switch (operation) {

        case 1:
            qDebug() << "\033[36m" << caller_and_function << "\033[0m" << message; break;

        case 2:
            qInfo() << "\033[32m" << caller_and_function << "\033[0m" << message; break;

        case 3:
            qWarning() << "\033[33m" << caller_and_function << "\033[0m" << message; break;

        case 4:
            qCritical() << "\033[31m" << caller_and_function << "\033[0m" << message; break;
        }
    }
    if( Debugger::debug_to_log ){ Logger::log( QString("%1,%2,%3").arg( caller_string ).arg( function ).arg( message ) , "Debugger" ); }
}

void Debugger::debug(const QObject *caller, QString function, QString message){
    Debugger::operation(caller , function , message , 1 );
}

void Debugger::info(const QObject *caller, QString function, QString message){
    Debugger::operation(caller , function , message , 2 );
}

void Debugger::warning(const QObject *caller, QString function, QString message){
    Debugger::operation(caller , function , message , 3 );
}

void Debugger::critical(const QObject *caller, QString function, QString message){
    Debugger::operation(caller , function , message , 4 );
}
