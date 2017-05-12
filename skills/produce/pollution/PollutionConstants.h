#ifndef POLLUTIONCONSTANTS
#define POLLUTIONCONSTANTS

#include <QDebug>

class PollutionConstants : public QObject
{
    Q_OBJECT
    Q_ENUMS(pollutant_types)

public:

    enum pollutant_types {CC, //
                            CH4,
                            CO, // Carbon monoxide
                            CO2, // Carbon dioxide
                            DALY, //
                            FC, // Fuel consumption
                            HC, // Hydrocarbons
                            NH3,
                            NOx, // Nitrogen oxide
                            N2O, // Nitrous oxide
                            PM, // Particle matters
                            PN, // Partikel number
                            SO2, // Sulphur dioxide.
                            WH // Watts per hour
                           };



    static PollutionConstants::pollutant_types stringToPollutant(QString type){
        QString lowered = type.toLower();
        if( lowered == "cc") return PollutionConstants::CC;
        if( lowered == "ch4") return PollutionConstants::CH4;
        if( lowered == "co") return PollutionConstants::CO;
        if( lowered == "co2") return PollutionConstants::FC;
        if( lowered == "daly") return PollutionConstants::DALY;
        if( lowered == "fc") return PollutionConstants::FC;
        if( lowered == "hc") return PollutionConstants::HC;
        if( lowered == "nh3") return PollutionConstants::NH3;
        if( lowered == "nox") return PollutionConstants::NOx;
        if( lowered == "n2o") return PollutionConstants::N2O;
        if( lowered == "pm") return PollutionConstants::PM;
        if( lowered == "pn") return PollutionConstants::PN;
        if( lowered == "so2") return PollutionConstants::SO2;
        if( lowered == "wh") return PollutionConstants::WH;

        qWarning() << "Unknown pollutant type" << lowered << endl;
        return PollutionConstants::FC;
    }

    static QString pollutantToString(PollutionConstants::pollutant_types type){
        if( type == PollutionConstants::CC ) return "CC";
        if( type == PollutionConstants::CH4 ) return "CH4";
        if( type == PollutionConstants::CO ) return "CO";
        if( type == PollutionConstants::CO2 ) return "CO2";
        if( type == PollutionConstants::DALY ) return "DALY";
        if( type == PollutionConstants::FC ) return "FC";
        if( type == PollutionConstants::HC ) return "HC";
        if( type == PollutionConstants::NH3) return "NH3";
        if( type == PollutionConstants::NOx) return "NOx";
        if( type == PollutionConstants::N2O) return "N2O";
        if( type == PollutionConstants::PM ) return "PM";
        if( type == PollutionConstants::PN ) return "PN";
        if( type == PollutionConstants::SO2) return "SO2";
        if( type == PollutionConstants::WH) return "WH";

        qWarning() << "Unknown contaminant type" << type << endl;
        return "";
    }
};

Q_DECLARE_METATYPE(PollutionConstants::pollutant_types)

#endif // POLLUTIONCONSTANTS

