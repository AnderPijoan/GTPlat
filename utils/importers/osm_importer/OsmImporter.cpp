#include "OsmImporter.h"

#include <QFile>

#include "environment/Environment.h"

OsmImporter::OsmImporter(QString file_path , QObject *parent) : Importer( file_path , parent ){

}

/**********************************************************************
 GETTERS
**********************************************************************/

/**
 * @brief OsmImporter::getOsmElements
 * The file is parsed everytime this method is called to create new geometries
 * IMPORTANT CALLER GETS OWNERSHIP OF GEOMETRIES AND HAS TO DELETE THEM IF NOT IN USE
 * @return
 */
QList<OsmImporter::OsmElement> OsmImporter::getOsmElements( const GSSGeometryController* environment , bool nodes , bool ways , bool relations){

    QList<OsmElement> elements;

    // QMAP<ID , ELEMENT>
    QMap<QString , OsmImporter::OsmElement> parsed_nodes;
    QMap<QString , OsmImporter::OsmElement> parsed_ways;
    QMap<QString , OsmImporter::OsmElement> parsed_relations;

    // Cant ensure they will be in order, so repeat the loop 3 times
    QFile file( this->importer_path );
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){

        // Nodes
        QXmlStreamReader* xml_nodes = new QXmlStreamReader(&file);

        while( !xml_nodes->atEnd() ){
            xml_nodes->readNextStartElement();
            if ( xml_nodes->name() == "node" ){
                OsmElement node = this->processNode( environment , xml_nodes );
                parsed_nodes.insert( node.id , node );
            }
        }
        if ( xml_nodes->hasError() ) {
            qWarning() << Q_FUNC_INFO << "Error parsing OSM XML for nodes" << xml_nodes->errorString() << endl;
        }
        delete xml_nodes;
        file.close();
    }


    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        // Ways
        QXmlStreamReader* xml_ways = new QXmlStreamReader(&file);

        while( !xml_ways->atEnd() ){
            xml_ways->readNextStartElement();
            if ( xml_ways->name() == "way" ){
                OsmElement way = this->processWay( environment , xml_ways , parsed_nodes );
                parsed_ways.insert( way.id , way );
            }
        }
        if ( xml_ways->hasError() ) {
            qWarning() << Q_FUNC_INFO << "Error parsing OSM XML for ways" << xml_ways->errorString() << endl;
        }
        delete xml_ways;
        file.close();
    }


    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        // Relations
        QXmlStreamReader* xml_rels = new QXmlStreamReader(&file);

        while( !xml_rels->atEnd() ){
            xml_rels->readNextStartElement();
            if ( xml_rels->name() == "relation" ){
                //OsmElement relation = this->processRelation( parsed_nodes , parsed_ways );
                //parsed_relations.insert( relation.id , relation );
            }
        }
        if ( xml_rels->hasError() ) {
            qWarning() << Q_FUNC_INFO << "Error parsing OSM XML for relations" << xml_rels->errorString() << endl;
        }
        delete xml_rels;
        file.close();
    }


    foreach (OsmElement e , parsed_nodes.values()) {
        if( nodes ){
            elements.append( e );
        } else {
            delete e.geometry;
        }
    }
    foreach (OsmElement e , parsed_ways.values()) {
        if( ways ){
            elements.append( e );
        } else {
            delete e.geometry;
        }
    }
    foreach (OsmElement e , parsed_relations.values()) {
        if( relations ){
            elements.append( e );
        } else {
            delete e.geometry;
        }
    }
    return elements;
}


OsmImporter::OsmElement OsmImporter::processNode( const GSSGeometryController* environment , QXmlStreamReader* xml ){
    OsmImporter::OsmElement node;
    if ( !xml->isStartElement() || xml->name() != "node"){
        return node;
    }

    node.id = xml->attributes().value("id").toString();
    node.type = "node";
    double x = xml->attributes().value("lon").toDouble( 0 );
    double y = xml->attributes().value("lat").toDouble( 0 );
    double z = xml->attributes().value("ele").toDouble( 0 );

    while ( xml->readNextStartElement() ) {
        if ( xml->name() == "tag"){
            node.tags.insert( xml->attributes().value("k").toString() , xml->attributes().value("v").toString() );
        }
        xml->skipCurrentElement();
    }

    node.geometry = environment->createPoint( GSSCoordinate(x , y , z) );
    return node;
}

OsmImporter::OsmElement OsmImporter::processWay( const GSSGeometryController* environment , QXmlStreamReader* xml , QMap<QString , OsmImporter::OsmElement> nodes ){
    OsmImporter::OsmElement way;
    if ( !xml->isStartElement() || xml->name() != "way"){
        return way;
    }

    way.id = xml->attributes().value("id").toString();
    way.type = "way";

    QList<GSSCoordinate> coors;

    while ( xml->readNextStartElement() ) {
        if ( xml->name() == "tag"){
            way.tags.insert( xml->attributes().value("k").toString() , xml->attributes().value("v").toString() );
        }
        if ( xml->name() == "nd"){
            QString ref = xml->attributes().value("ref").toString();
            if( nodes.keys().contains( ref ) ){
                OsmImporter::OsmElement node = nodes.value( ref );
                coors.append( GSSCoordinate( node.geometry->getRepresentativeCoordinate().getX() , node.geometry->getRepresentativeCoordinate().getY() , node.geometry->getRepresentativeCoordinate().getZ() ) );
            }
        }
        xml->skipCurrentElement();
    }

    if( coors.at(0) != coors.at( coors.size()-1 ) ){ // Check if line
        way.geometry = environment->createLineString( coors );
    } else {                                            // Or closed polygon
        QList< QList<GSSCoordinate> > rings_coors;
        rings_coors.append( coors );
        way.geometry = environment->createPolygon( rings_coors );
    }

    return way;
}

OsmImporter::OsmElement OsmImporter::processRelation( const GSSGeometryController* environment , QXmlStreamReader* xml , QMap<QString, OsmElement> nodes, QMap<QString, OsmElement> ways){
    Q_UNUSED(environment)
    Q_UNUSED(nodes)
    Q_UNUSED(ways)

    OsmImporter::OsmElement relation;
    if ( !xml->isStartElement() || xml->name() != "relation"){
        return relation;
    }

    relation.id = xml->attributes().value("id").toString();
    relation.type = "relation";

    while ( xml->readNextStartElement() ) {
        if ( xml->name() == "tag"){
            relation.tags.insert( xml->attributes().value("k").toString() , xml->attributes().value("v").toString() );
        }
        if ( xml->name() == "nd"){

        }
        xml->skipCurrentElement();
    }

    return relation;
}
