#ifndef ROADCONSTANTS_H
#define ROADCONSTANTS_H

#include <QObject>

#include "utils/exporters/debugger/Debugger.h"
#include "utils/units/UnitConstants.h"

class RoadConstants : public QObject
{
    Q_OBJECT
    Q_ENUMS(road_types)
public:

    enum road_types         {

        UNCLASSIFIED = 0,

        // HIGHWAY
        MOTORWAY ,
        MOTORWAY_LINK ,
        MOTORWAY_JUNCTION ,
        TRUNK ,
        TRUNK_LINK ,
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
        CYCLEWAY ,
        BRIDLEWAY ,
        ROAD ,

        // JUNCTION
        ROUNDABOUT ,

        // CYCLEWAY
        CYCLE_LANE ,
        CYCLE_TRACK ,
        CYCLE_OPPOSITE_TRACK ,
        CYCLE_OPPOSITE ,
    };

    static RoadConstants::road_types fromOSM(QString k, QString v){
        QString key = k.toLower().trimmed();
        QString value = v.toLower().trimmed();
        if( key == "highway"){
            if( value == "motorway") return RoadConstants::MOTORWAY;
            if( value == "motorway_link") return RoadConstants::MOTORWAY_LINK;
            if( value == "motorway_junction") return RoadConstants::MOTORWAY_JUNCTION;
            if( value == "trunk") return RoadConstants::TRUNK;
            if( value == "trunk_link") return RoadConstants::TRUNK_LINK;
            if( value == "primary") return RoadConstants::PRIMARY;
            if( value == "primary_link") return RoadConstants::PRIMARY_LINK;
            if( value == "secondary") return RoadConstants::SECONDARY;
            if( value == "secondary_link") return RoadConstants::SECONDARY_LINK;
            if( value == "tertiary") return RoadConstants::TERTIARY;
            if( value == "tertiary_link") return RoadConstants::TERTIARY_LINK;
            if( value == "residential") return RoadConstants::RESIDENTIAL;
            if( value == "road") return RoadConstants::ROAD;
            if( value == "living_street") return RoadConstants::LIVING_STREET;
            if( value == "service") return RoadConstants::SERVICE;
            if( value == "track") return RoadConstants::TRACK;
            if( value == "path") return RoadConstants::PATH;
            if( value == "unclassified") return RoadConstants::UNCLASSIFIED;
            if( value == "cycleway") return RoadConstants::CYCLEWAY;

        } if( key == "junction"){
            if( value == "roundabout") return RoadConstants::ROUNDABOUT;

        } if( key == "cycleway"){
            if( value == "cycleway") return RoadConstants::CYCLEWAY;
            if( value == "cycle_lane") return RoadConstants::CYCLE_LANE;
            if( value == "cycle_track") return RoadConstants::CYCLE_TRACK;
            if( value == "track") return RoadConstants::CYCLE_TRACK;
            if( value == "cycle_opposite_track") return RoadConstants::CYCLE_OPPOSITE_TRACK;
            if( value == "cycle_opposite") return RoadConstants::CYCLE_OPPOSITE;
            if( value == "bridleway") return RoadConstants::BRIDLEWAY;
        }

        Debugger::debug( 0 , Q_FUNC_INFO , QString("Railway key-value not matched key: %1 value: %2").arg( key ).arg( value ) );
        return RoadConstants::UNCLASSIFIED;
    }

    static QString toString(RoadConstants::road_types type){
        switch (type){
        case RoadConstants::MOTORWAY: return "motorway";
        case RoadConstants::MOTORWAY_LINK: return "motorway_link";
        case RoadConstants::MOTORWAY_JUNCTION: return "motorway_junction";
        case RoadConstants::TRUNK: return "trunk";
        case RoadConstants::TRUNK_LINK: return "trunk_link";
        case RoadConstants::PRIMARY: return "primary";
        case RoadConstants::PRIMARY_LINK: return "primary_link";
        case RoadConstants::SECONDARY: return "secondary";
        case RoadConstants::SECONDARY_LINK: return "secondary_link";
        case RoadConstants::TERTIARY: return "tertiary";
        case RoadConstants::TERTIARY_LINK: return "tertiary_link";
        case RoadConstants::ROUNDABOUT: return "roundabout";
        case RoadConstants::RESIDENTIAL: return "residential";
        case RoadConstants::LIVING_STREET: return "living_street";
        case RoadConstants::SERVICE: return "service";
        case RoadConstants::TRACK: return "track";
        case RoadConstants::PATH: return "path";
        case RoadConstants::CYCLEWAY: return "cycleway";
        case RoadConstants::BRIDLEWAY: return "bridleway";
        case RoadConstants::UNCLASSIFIED: return "unclassified";
        case RoadConstants::ROAD: return "road";
        case RoadConstants::CYCLE_LANE: return "cycle_lane";
        case RoadConstants::CYCLE_TRACK: return "cycle_track";
        case RoadConstants::CYCLE_OPPOSITE_TRACK: return "cycle_opposite_track";
        case RoadConstants::CYCLE_OPPOSITE: return "cycle_opposite";
        }
        return "";
    }

    static SpeedUnit defaultSpeed(RoadConstants::road_types type){
        switch (type){
        case RoadConstants::MOTORWAY:
            return SpeedUnit( 120 , KilometerPerHour );
        case RoadConstants::MOTORWAY_LINK:
        case RoadConstants::MOTORWAY_JUNCTION:
        case RoadConstants::TRUNK:
            return SpeedUnit( 100 , KilometerPerHour );
        case RoadConstants::TRUNK_LINK:
        case RoadConstants::PRIMARY:
            return SpeedUnit( 90 , KilometerPerHour );
        case RoadConstants::PRIMARY_LINK:
        case RoadConstants::SECONDARY:
            return SpeedUnit( 80 , KilometerPerHour );
        case RoadConstants::SECONDARY_LINK:
        case RoadConstants::TERTIARY:
        case RoadConstants::ROUNDABOUT:
            return SpeedUnit( 70 , KilometerPerHour );
        case RoadConstants::TERTIARY_LINK:
        case RoadConstants::RESIDENTIAL:
        case RoadConstants::ROAD:
            return SpeedUnit( 50 , KilometerPerHour );
        case RoadConstants::LIVING_STREET:
        case RoadConstants::SERVICE:
            return SpeedUnit( 30 , KilometerPerHour );
        case RoadConstants::TRACK:
        case RoadConstants::PATH:
        case RoadConstants::UNCLASSIFIED:
            return SpeedUnit( 4 , KilometerPerHour );
        case RoadConstants::CYCLEWAY:
        case RoadConstants::CYCLE_LANE:
        case RoadConstants::CYCLE_TRACK:
        case RoadConstants::CYCLE_OPPOSITE_TRACK:
        case RoadConstants::CYCLE_OPPOSITE:
        case RoadConstants::BRIDLEWAY:
            return SpeedUnit( 40 , KilometerPerHour );
        }
        return SpeedUnit( 4 , KilometerPerHour );
    }
};


#endif // ROADCONSTANTS_H

