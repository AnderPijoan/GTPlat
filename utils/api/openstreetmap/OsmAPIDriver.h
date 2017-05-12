#ifndef OSMAPIDRIVER_H
#define OSMAPIDRIVER_H

#include <QObject>
#include <QList>
#include <QMap>

#include "utils/geometry/GSSGeometryController.h"
#include "geos/geom/Geometry.h"

#include "utils/api/SyncAPIDriver.h"
#include "utils/importers/osm_importer/OsmImporter.h"

using namespace geos::geom;

class OsmAPIDriver : public QObject
{
    Q_OBJECT
public:
    explicit OsmAPIDriver( const GSSGeometryController* geometry_controller , QObject *parent = 0);
    ~OsmAPIDriver();

    // GETTERS

    /**
     * The download is done and parsed everytime this method is called to create new geometries
     * IMPORTANT CALLER GETS OWNERSHIP OF GEOMETRIES AND HAS TO DELETE THEM IF NOT IN USE
     * @param bounds
     * @param query
     * @return
     */
    QList<OsmImporter::OsmElement> getOsmElements(Geometry* bounds , QMap<QString, QString> tags , bool nodes = true , bool ways = true , bool rels = true );

private:
    const GSSGeometryController* geometry_controller;
};

#endif // OSMAPIDRIVER_H
