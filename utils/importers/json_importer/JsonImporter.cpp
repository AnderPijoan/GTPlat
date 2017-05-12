#include "JsonImporter.h"

#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

#include "utils/exporters/debugger/Debugger.h"

JsonImporter::JsonImporter(QString file_path, QObject *parent) : Importer( file_path , parent ){
}

/**********************************************************************
 GETTERS
**********************************************************************/

QJsonObject JsonImporter::getJson(){
    QFile file(this->importer_path);
    QJsonObject json;
    QJsonParseError jerror;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QJsonDocument json_document = QJsonDocument::fromJson(file.readAll(), &jerror);
        if(jerror.error != QJsonParseError::NoError){
            Debugger::warning( this , Q_FUNC_INFO , QString("Error when parsing JSON file %1").arg( jerror.errorString() ) );
        } else {
            json = json_document.object();
        }
        file.close();
    } else {
        Debugger::warning( this , Q_FUNC_INFO , QString("Can't open JSON file %1").arg( this->file_path ) );
    }

    return json;
}

