#include "Importer.h"

#include <QDir>
#include <QFile>
#include "utils/exporters/debugger/Debugger.h"

Importer::Importer(QString import_path, QObject *parent) : QObject( parent ){
    this->importer_path = import_path;

    QDir dir( import_path );
    if( dir.exists() ){
        return;
    }

    QFile file( import_path );
    if( file.exists() ){
        return;
    }

    Debugger::warning( this , Q_FUNC_INFO , QString("File or Directory %1 not found").arg( import_path ) );
}

