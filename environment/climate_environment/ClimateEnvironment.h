#ifndef CLIMATEENVIRONMENT_H
#define CLIMATEENVIRONMENT_H

#include <QObject>

#include "utils/grid/GSSGrid.h"
#include "utils/geometry/GSSCoordinate.h"
#include "utils/units/UnitConstants.h"

QT_FORWARD_DECLARE_CLASS(Environment)

class ClimateEnvironment : public QObject
{
    Q_OBJECT
    friend class Environment; // Environment can access the constructor

public:
    QJsonObject toJSON();
    void fromJSON(QJsonObject json);

    // GETTERS
    TemperatureUnit getTemperature( GSSCoordinate coor ) const;
    double getHumidity( GSSCoordinate coor ) const;
    double getSunIrradiation( GSSCoordinate coor ) const;

    // SETTERS
    void addTemperature( GSSCoordinate coor , TemperatureUnit value );
    void addHumidity( GSSCoordinate coor , double value );
    void addSunIrradiation( GSSCoordinate coor , double value );

private:
    explicit ClimateEnvironment(Environment *parent);
    ~ClimateEnvironment();

    // Environment temperatures
    GSSGrid temperature;

    // Environment humidity
    GSSGrid humidity;

    // Environment sun irradiation
    GSSGrid sun_irradiation;

};

#endif // CLIMATEENVIRONMENT_H
