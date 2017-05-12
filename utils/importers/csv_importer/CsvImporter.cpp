#include "CsvImporter.h"
#include <QDebug>
#include <QDir>

#include <QFile>
#include <QTextStream>

CsvImporter::CsvImporter(QString file_path , char separator, QObject *parent) : Importer( file_path , parent ){
    this->separator = separator;
}

/**********************************************************************
 GETTERS
**********************************************************************/

QMap<QString, int> CsvImporter::getColumnNames(){

    QFile file(this->importer_path);
    QTextStream in(&file);
    QMap<QString, int> column_names;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QList<QString> header = in.readLine().split(this->separator);
        for ( int i=0 ; i < header.size() ; i++ ) {
            column_names.insert(header.at(i).toUpper(), i);
        }
        file.close();
    }
    return column_names;
}


QList< QMap<QString , QString> > CsvImporter::getRows(){
    QFile file(this->importer_path);
    QTextStream in(&file);

    QMap<QString, int> column_names = this->getColumnNames();
    QList< QMap<QString , QString> > results;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        in.readLine(); // Jump first line
        while (!in.atEnd()) {
            QMap<QString , QString> m;
            QList<QString> rows = in.readLine().split(this->separator);
            foreach( QString column_name , column_names.keys() ){
                m.insert( column_name , rows.at( column_names.value( column_name ) ) );
            }
            results.append( m );
        }
        file.close();
    }
    return results;
}
