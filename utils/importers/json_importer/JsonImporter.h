#ifndef JSONIMPORTER_H
#define JSONIMPORTER_H

#include <QObject>
#include <QJsonObject>

#include "utils/importers/Importer.h"

class JsonImporter : public Importer
{
    Q_OBJECT

public:
    explicit JsonImporter(QString file_path , QObject *parent = 0);

    // GETTERS
    QJsonObject getJson();

private:
    QString file_path;
};

#endif // JSONIMPORTER_H
