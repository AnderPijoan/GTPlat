#ifndef UNIFORMDISTRIBUTION_H
#define UNIFORMDISTRIBUTION_H

#include <QObject>

class UniformDistribution : public QObject
{
    Q_OBJECT

public:
    UniformDistribution(QObject* parent);
    ~UniformDistribution();

    // METHODS
    double uniformDistribution();

private:
    std::uniform_real_distribution<double> uniform_distribution;
};

#endif // UNIFORMDISTRIBUTION_H
