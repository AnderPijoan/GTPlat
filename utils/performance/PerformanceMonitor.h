#ifndef PERFORMANCEMONITOR_H
#define PERFORMANCEMONITOR_H

#include <QObject>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QTimer>

#include "environment/Environment.h"

class PerformanceMonitor : public QObject
{
    Q_OBJECT

public:

    PerformanceMonitor( QCoreApplication* app , Environment* environment );

    // GETTERS

public slots:
    void startWatchingPerformance();
    void stopWatchingPerformance();

private slots:
    void storeCurrentPerformance();

private:

    // METHODS
    QStringList getSystemInfo();
    QStringList getUsage();

    // Attributes
    Environment* environment;
    QElapsedTimer elapsed_time;
    QTimer* timer;

    // Stored variables
    int max_threads_count;
    int max_running_agents_count;

};

#endif // PERFORMANCEMONITOR_H
