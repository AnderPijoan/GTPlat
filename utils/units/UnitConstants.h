#ifndef UNITSCONSTANTS
#define UNITSCONSTANTS

#include <QObject>

#include <KUnitConversion/Value>
#include <KUnitConversion/Converter>

using namespace KUnitConversion;

/**********************************************************************
 BASE UNIT CLASS
**********************************************************************/

class BaseUnit : public KUnitConversion::Value {

public:
    BaseUnit() : KUnitConversion::Value(){}
    BaseUnit(qreal number, const Unit &unit) : KUnitConversion::Value(number , unit){}
    BaseUnit(qreal number, const QString &unitString) : KUnitConversion::Value(number , unitString){}
    BaseUnit(qreal number, KUnitConversion::UnitId id) : KUnitConversion::Value(number , id){}
    BaseUnit(const QVariant &number, const QString &unitString) : KUnitConversion::Value(number , unitString){}
    BaseUnit(const BaseUnit &other) : KUnitConversion::Value(other){}

    // COMPARE
    bool operator <(const BaseUnit &other) const{ return this->convertTo( other.unit() ).number() < other.number(); }
    bool operator <=(const BaseUnit &other) const{ return this->convertTo( other.unit() ).number() <= other.number(); }
    bool operator >(const BaseUnit &other) const{ return this->convertTo( other.unit() ).number() > other.number(); }
    bool operator >=(const BaseUnit &other) const{ return this->convertTo( other.unit() ).number() >= other.number(); }

    // MATH
    BaseUnit operator +(const BaseUnit &other) const{ return BaseUnit( this->number() + BaseUnit::convert( other.number() , other.unit().id() , this->unit().id() ) , this->unit() ); }
    BaseUnit operator +(const double v) const{ return BaseUnit( this->number() + v , this->unit() ); }

    BaseUnit operator -(const BaseUnit &other) const{ return BaseUnit( this->number() - BaseUnit::convert( other.number() , other.unit().id() , this->unit().id() ) , this->unit() ); }
    BaseUnit operator -(const double v) const{ return BaseUnit( this->number() - v , this->unit() ); }

    BaseUnit operator *(const double v) const{ return BaseUnit( this->number() * v , this->unit() ); }
    BaseUnit operator /(const double v) const{ return BaseUnit( this->number() / v , this->unit() ); }

    // STATIC
    static qreal convert( qreal number , KUnitConversion::UnitId from , KUnitConversion::UnitId to ){
        return BaseUnit( number , from ).convertTo( to ).number();
    }

};

/**********************************************************************
 PRICE IN EUROS
**********************************************************************/

class CurrencyUnit : public BaseUnit {
public:
    CurrencyUnit(const BaseUnit &other) : BaseUnit(other){ Q_ASSERT(other.unit().id() == KUnitConversion::Eur ); }
    CurrencyUnit(qreal number = 0) : BaseUnit( number , KUnitConversion::Eur ){}
    CurrencyUnit(qreal number , KUnitConversion::UnitId id) : CurrencyUnit( BaseUnit::convert( number , id , KUnitConversion::Eur ) ){}
};

/**********************************************************************
 MASS IN GRAMS
**********************************************************************/

class MassUnit : public BaseUnit {
public:
    MassUnit(const BaseUnit &other) : BaseUnit(other){ Q_ASSERT(other.unit().id() == KUnitConversion::Gram ); }
    MassUnit(qreal number = 0) : BaseUnit( number , KUnitConversion::Gram ){}
    MassUnit(qreal number , KUnitConversion::UnitId id) : MassUnit( BaseUnit::convert( number , id , KUnitConversion::Gram ) ){}
};

/**********************************************************************
 TIME IN SECONDS
**********************************************************************/

class TimeUnit : public BaseUnit {
public:
    TimeUnit(const BaseUnit &other) : BaseUnit(other){ Q_ASSERT(other.unit().id() == KUnitConversion::Second ); }
    TimeUnit(qreal number = 0) : BaseUnit( number , KUnitConversion::Second ){}
    TimeUnit(qreal number , KUnitConversion::UnitId id) : TimeUnit( BaseUnit::convert( number , id , KUnitConversion::Second ) ){}
};

/**********************************************************************
 SPEED IN METERS PER SECOND
**********************************************************************/

class SpeedUnit : public BaseUnit {
public:
    SpeedUnit(const BaseUnit &other) : BaseUnit(other){ Q_ASSERT(other.unit().id() == KUnitConversion::MeterPerSecond ); }
    SpeedUnit(qreal number = 0) : BaseUnit( number , KUnitConversion::MeterPerSecond ){}
    SpeedUnit(qreal number , KUnitConversion::UnitId id) : SpeedUnit( BaseUnit::convert( number , id , KUnitConversion::MeterPerSecond ) ){}
};

/**********************************************************************
 LENGTH IN METERS
**********************************************************************/

class LengthUnit : public BaseUnit {
public:
    LengthUnit(const BaseUnit &other) : BaseUnit(other){ Q_ASSERT(other.unit().id() == KUnitConversion::Meter ); }
    LengthUnit(qreal number = 0) : BaseUnit( number , KUnitConversion::Meter ){}
    LengthUnit(qreal number , KUnitConversion::UnitId id) : LengthUnit( BaseUnit::convert( number , id , KUnitConversion::Meter ) ){}
};

/**********************************************************************
 TEMPERATURE IN CELSIUS DEGREES
**********************************************************************/

class TemperatureUnit : public BaseUnit {
public:
    TemperatureUnit(const BaseUnit &other) : BaseUnit(other){ Q_ASSERT(other.unit().id() == KUnitConversion::Celsius ); }
    TemperatureUnit(qreal number = 0) : BaseUnit( number , KUnitConversion::Celsius ){}
    TemperatureUnit(qreal number , KUnitConversion::UnitId id) : TemperatureUnit( BaseUnit::convert( number , id , KUnitConversion::Celsius ) ){}
};

/**********************************************************************
 AREA IN SQUARE METERS
**********************************************************************/

class AreaUnit : public BaseUnit {
public:
    AreaUnit(const BaseUnit &other) : BaseUnit(other){ Q_ASSERT(other.unit().id() == KUnitConversion::SquareMeter ); }
    AreaUnit(qreal number = 0) : BaseUnit( number , KUnitConversion::SquareMeter ){}
    AreaUnit(qreal number , KUnitConversion::UnitId id) : AreaUnit( BaseUnit::convert( number , id , KUnitConversion::SquareMeter ) ){}
};

/**********************************************************************
 POWER IN SQUARE WATTS
**********************************************************************/

class PowerUnit : public BaseUnit {
public:
    PowerUnit(const BaseUnit &other) : BaseUnit(other){ Q_ASSERT(other.unit().id() == KUnitConversion::Watt ); }
    PowerUnit(qreal number = 0) : BaseUnit( number , KUnitConversion::Watt ){}
    PowerUnit(qreal number , KUnitConversion::UnitId id) : PowerUnit( BaseUnit::convert( number , id , KUnitConversion::Watt ) ){}
};


/**********************************************************************
 FREQUENCY UNITS IN HERTZ
**********************************************************************/

class FrequencyUnit : public BaseUnit {
public:
    FrequencyUnit(const BaseUnit &other) : BaseUnit(other){ Q_ASSERT(other.unit().id() == KUnitConversion::Hertz ); }
    FrequencyUnit(qreal number = 0) : BaseUnit( number , KUnitConversion::Hertz ){}
    FrequencyUnit(qreal number , KUnitConversion::UnitId id) : FrequencyUnit( BaseUnit::convert( number , id , KUnitConversion::Hertz ) ){}
};


/**********************************************************************
 VOLUME UNITS IN CUBIC METERS
**********************************************************************/

class VolumeUnit : public BaseUnit {
public:
    VolumeUnit(const BaseUnit &other) : BaseUnit(other){ Q_ASSERT(other.unit().id() == KUnitConversion::CubicMeter ); }
    VolumeUnit(qreal number = 0) : BaseUnit( number , KUnitConversion::CubicMeter ){}
    VolumeUnit(qreal number , KUnitConversion::UnitId id) : VolumeUnit( BaseUnit::convert( number , id , KUnitConversion::CubicMeter ) ){}
};


#endif // UNITSCONSTANTS

