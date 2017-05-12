#ifndef RAILWAYCONSTANTS
#define RAILWAYCONSTANTS

#include <QObject>

#include "utils/exporters/debugger/Debugger.h"
#include "utils/units/UnitConstants.h"

class RailwayConstants : public QObject
{
    Q_OBJECT
    Q_ENUMS(railway_types)
public:

    enum railway_types         {

        UNCLASSIFIED = 0,

        // RAILWAY
        FUNICULAR = 1,
        LIGHT_RAIL = 2,
        NARROW_GAUGE = 3,
        PRESERVED = 4,
        RAIL = 5,
        SUBWAY = 6,
        TRAM = 7,
    };

    static RailwayConstants::railway_types fromOSM(QString k, QString v){
        QString key = k.toLower().trimmed();
        QString value = v.toLower().trimmed();
        if( key == "railway"){
            if( value == "funicular") return RailwayConstants::FUNICULAR;
            if( value == "light_rail") return RailwayConstants::LIGHT_RAIL;
            if( value == "narrow_gauge") return RailwayConstants::NARROW_GAUGE;
            if( value == "tram") return RailwayConstants::TRAM;
            if( value == "subway") return RailwayConstants::SUBWAY;
            if( value == "preserved") return RailwayConstants::PRESERVED;
            if( value == "rail") return RailwayConstants::RAIL;
        }

        Debugger::debug( 0 , Q_FUNC_INFO , QString("Railway key-value not matched key: %1 value: %2").arg( key ).arg( value ) );
        return RailwayConstants::UNCLASSIFIED;
    }

    static QString toString(RailwayConstants::railway_types type){
        switch (type){
        case RailwayConstants::UNCLASSIFIED: return "unclassified";
        case RailwayConstants::FUNICULAR: return "funicular";
        case RailwayConstants::LIGHT_RAIL: return "light_rail";
        case RailwayConstants::NARROW_GAUGE: return "narrow_gauge";
        case RailwayConstants::PRESERVED: return "preserved";
        case RailwayConstants::RAIL: return "rail";
        case RailwayConstants::SUBWAY: return "subway";
        case RailwayConstants::TRAM: return "tram";
        }
        return "";
    }

    static SpeedUnit defaultSpeed(RailwayConstants::railway_types type){
        switch (type){
        case RailwayConstants::UNCLASSIFIED:
            return SpeedUnit( 5 , KilometerPerHour );

        case RailwayConstants::FUNICULAR:
            return SpeedUnit( 50 , KilometerPerHour );

        case RailwayConstants::LIGHT_RAIL:
        case RailwayConstants::NARROW_GAUGE:
        case RailwayConstants::TRAM:
        case RailwayConstants::SUBWAY:
            return SpeedUnit( 90 , KilometerPerHour  );
        case RailwayConstants::PRESERVED:
            return SpeedUnit( 50 , KilometerPerHour );
        case RailwayConstants::RAIL:
            return SpeedUnit( 150 , KilometerPerHour );
        }
        return SpeedUnit( 5 , KilometerPerHour );
    }

};

#endif // RAILWAYCONSTANTS

