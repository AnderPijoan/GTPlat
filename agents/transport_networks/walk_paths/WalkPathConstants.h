#ifndef WALKPATHCONSTANTS
#define WALKPATHCONSTANTS

#include <QObject>

#include "utils/exporters/debugger/Debugger.h"
#include "utils/units/UnitConstants.h"

class WalkPathConstants : public QObject
{
    Q_OBJECT
    Q_ENUMS(walk_path_types)
public:

    enum walk_path_types         {

        UNCLASSIFIED = 0,

        // HIGHWAY
        PRIMARY ,
        PRIMARY_LINK ,
        SECONDARY ,
        SECONDARY_LINK ,
        TERTIARY ,
        TERTIARY_LINK ,
        RESIDENTIAL ,
        LIVING_STREET ,
        SERVICE ,
        TRACK ,
        PATH ,
        PEDESTRIAN ,
        FOOTWAY ,
        STEPS ,
        ROAD ,

        // JUNCTION
        ROUNDABOUT = 18,
    };

    static WalkPathConstants::walk_path_types fromOSM(QString k, QString v){
        QString key = k.toLower().trimmed();
        QString value = v.toLower().trimmed();
        if( key == "highway"){
            if( value == "primary") return WalkPathConstants::PRIMARY;
            if( value == "primary_link") return WalkPathConstants::PRIMARY_LINK;
            if( value == "secondary") return WalkPathConstants::SECONDARY;
            if( value == "secondary_link") return WalkPathConstants::SECONDARY_LINK;
            if( value == "tertiary") return WalkPathConstants::TERTIARY;
            if( value == "tertiary_link") return WalkPathConstants::TERTIARY_LINK;
            if( value == "residential") return WalkPathConstants::RESIDENTIAL;
            if( value == "road") return WalkPathConstants::ROAD;
            if( value == "living_street") return WalkPathConstants::LIVING_STREET;
            if( value == "service") return WalkPathConstants::SERVICE;
            if( value == "track") return WalkPathConstants::TRACK;
            if( value == "path") return WalkPathConstants::PATH;
            if( value == "unclassified") return WalkPathConstants::UNCLASSIFIED;
            if( value == "pedestrian") return WalkPathConstants::PEDESTRIAN;
            if( value == "footway") return WalkPathConstants::FOOTWAY;
            if( value == "steps") return WalkPathConstants::STEPS;

        } if( key == "junction"){
            if( value == "roundabout") return WalkPathConstants::ROUNDABOUT;

        }

        Debugger::debug( 0 , Q_FUNC_INFO , QString("Railway key-value not matched key: %1 value: %2").arg( key ).arg( value ) );
        return WalkPathConstants::UNCLASSIFIED;
    }

    static QString toString(WalkPathConstants::walk_path_types type){
        switch (type){
        case WalkPathConstants::PRIMARY: return "primary";
        case WalkPathConstants::PRIMARY_LINK: return "primary_link";
        case WalkPathConstants::SECONDARY: return "secondary";
        case WalkPathConstants::SECONDARY_LINK: return "secondary_link";
        case WalkPathConstants::TERTIARY: return "tertiary";
        case WalkPathConstants::TERTIARY_LINK: return "tertiary_link";
        case WalkPathConstants::ROUNDABOUT: return "roundabout";
        case WalkPathConstants::RESIDENTIAL: return "residential";
        case WalkPathConstants::LIVING_STREET: return "living_street";
        case WalkPathConstants::SERVICE: return "service";
        case WalkPathConstants::TRACK: return "track";
        case WalkPathConstants::PATH: return "path";
        case WalkPathConstants::PEDESTRIAN: return "pedestrian";
        case WalkPathConstants::FOOTWAY: return "footway";
        case WalkPathConstants::STEPS: return "steps";
        case WalkPathConstants::UNCLASSIFIED: return "unclassified";
        case WalkPathConstants::ROAD: return "road";
        }
        return "";
    }

    static SpeedUnit defaultSpeed(WalkPathConstants::walk_path_types type){
        Q_UNUSED(type)
        return SpeedUnit( 5 ); // 5km/h
    }

};

#endif // WALKPATHCONSTANTS

