#ifndef OSMIMPORTER_H
#define OSMIMPORTER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QXmlStreamReader>

#include "utils/importers/Importer.h"
#include "utils/geometry/GSSGeometryController.h"

class OsmImporter : public Importer
{
    Q_OBJECT
public:

    // HELPER STRUCT
    struct OsmElement {
        QString id;
        QString type;
        QMap<QString, QString> tags;
        GSSGeometry* geometry;
    };

    explicit OsmImporter(QString file_path , QObject *parent = 0);

    // GETTERS
    QList<OsmElement> getOsmElements( const GSSGeometryController* environment , bool nodes = true , bool ways = true , bool relations = true );

private:

    // METHODS
    OsmElement processNode( const GSSGeometryController* environment , QXmlStreamReader* xml );
    OsmElement processWay( const GSSGeometryController* environment , QXmlStreamReader* xml , QMap<QString , OsmImporter::OsmElement> nodes );
    OsmElement processRelation( const GSSGeometryController* environment , QXmlStreamReader* xml , QMap<QString , OsmImporter::OsmElement> nodes , QMap<QString , OsmImporter::OsmElement> ways );
};

#endif // OSMIMPORTER_H
