#ifndef CSVIMPORTER_H
#define CSVIMPORTER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>

#include "utils/importers/Importer.h"

class CsvImporter : public Importer
{
    Q_OBJECT
public:
    explicit CsvImporter(QString file_path , char separator = ',', QObject *parent = 0);

    // GETTERS
    QMap<QString,int> getColumnNames();
    QList< QMap<QString , QString> > getRows();

private:
    char separator;

};

#endif // CSVIMPORTER_H
