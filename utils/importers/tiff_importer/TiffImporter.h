#ifndef TIFFIMPORTER_H
#define TIFFIMPORTER_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QFile>
#include <QMap>

#include "gdal/gdal.h"
#include "gdal/gdal_priv.h"
#include "gdal/cpl_conv.h"

#include "utils/importers/Importer.h"
#include "utils/grid/GSSGrid.h"

class TiffImporter : public Importer
{
    Q_OBJECT
public:
    explicit TiffImporter(QString file_path , QObject *parent = 0);

    // GETTERS
    GSSGrid getValues();

};

#endif // TIFFIMPORTER_H
