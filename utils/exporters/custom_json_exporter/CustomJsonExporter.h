#ifndef CUSTOMJSONEXPORTER_H
#define CUSTOMJSONEXPORTER_H

#include <QObject>
#include <QSharedPointer>
#include <QJsonObject>
#include <QList>

#include "utils/units/UnitConstants.h"
#include "utils/geometry/GSSGeometry.h"

QT_FORWARD_DECLARE_CLASS(Agent)
QT_FORWARD_DECLARE_CLASS(GSSGrid)

class CustomJsonExporter : public QObject
{
    Q_OBJECT
public:

    // SIMPLE ELEMENTS
    static QJsonObject format( QVariant value );
    static QJsonObject format( BaseUnit value );
    static QJsonObject format( GSSGeometry* geom , QJsonObject properties = QJsonObject() );

    // AGENT
    static QJsonObject format( Agent* agent );

    // GRID
    static QJsonObject format( GSSGrid grid );

    // MAP
    static QJsonObject format( QMap<QString , int> map , QString map_title = ""); // QMAP< QSTRING , DOUBLE > to BAR CHART
    static QJsonObject format( QMap<QString , double> map , QString map_title = ""); // QMAP< QSTRING , DOUBLE > to BAR CHART
};

#endif // CUSTOMJSONEXPORTER_H
