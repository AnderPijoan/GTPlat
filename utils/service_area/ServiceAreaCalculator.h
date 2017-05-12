#ifndef SERVICEAREACALCULATOR_H
#define SERVICEAREACALCULATOR_H

#include <QObject>

class ServiceAreaCalculator : public QObject
{
    Q_OBJECT

public:
    ServiceAreaCalculator( QObject* parent = 0 );
};

#endif // SERVICEAREACALCULATOR_H
