#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>
#include <QVector>

class Statistics : public QObject
{
    Q_OBJECT

public:
    explicit Statistics(QObject *parent = 0);

    // GETTERS
    static double mean(QVector<double> values);
    static double std(QVector<double> values);
    static double variance(QVector<double> values);
    static double max(QVector<double> values);
    static double min(QVector<double> values);

};

#endif // STATISTICS_H
