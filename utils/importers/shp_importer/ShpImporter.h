#ifndef SHPIMPORTER_H
#define SHPIMPORTER_H

#include "utils/importers/Importer.h"
#include "utils/geometry/GSSGeometryController.h"
#include "shapefil.h"

class ShpImporter : public Importer
{
    Q_OBJECT
public:

    explicit ShpImporter(QString path , QString filename, QObject* parent = 0);
    ~ShpImporter();

    // GETTERS
    int getShapeCount();
    GSSGeometry* getShape(int index , const GSSGeometryController* geometry_controller , QJsonObject &properties);

private:

    GSSGeometry* parseShape(SHPObject *obj , const GSSGeometryController* geometry_controller);

    SHPHandle shp_handle;
    DBFHandle dbf_handle;
    int shape_count;
    int shape_type;

};

#endif // SHPIMPORTER_H
