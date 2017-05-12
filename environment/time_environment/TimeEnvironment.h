#ifndef TIMEENVIRONMENT_H
#define TIMEENVIRONMENT_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>

#include "utils/units/UnitConstants.h"

QT_FORWARD_DECLARE_CLASS(Environment)

/**
 * @brief The TimeEnvironment class
 * Class to be used in environment to get time / hours.
 */
class TimeEnvironment : public QObject
{
    Q_OBJECT
    friend class Environment; // Environment can access the constructor

public:
    QJsonObject toJSON();
    void fromJSON(QJsonObject json);

    // GETTERS
    QDateTime getCurrentDateTime() const;
    double getTimeSpeed() const;

    // SETTERS
    void setDate( QDate& date );
    void setTime( QTime& time );
    void setDatetime(QDateTime datetime);
    void setTimeSpeed(double time_speed = 1);

private:
    explicit TimeEnvironment(Environment* parent);
    ~TimeEnvironment();

    // Environment start date/time expecified when init
    QDateTime datetime;

    // Time speed
    // (How many seconds will there be in a real world second)
    double time_speed;

};

#endif // TIMEENVIRONMENT_H
