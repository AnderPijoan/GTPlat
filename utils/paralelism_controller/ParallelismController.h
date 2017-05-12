#ifndef PARALLELISMCONTROLLER_H
#define PARALLELISMCONTROLLER_H

#include <QObject>
#include <QThreadPool>
#include <QMap>

class ParallelismController : public QObject
{
    Q_OBJECT
public:

    static ParallelismController* getInstance();

    // GETTERS
    int getThreadsCount() const;

    // METHODS
    QThread* getThread( int agent_id );
    QThread* liberateThread( QThread* thread );

signals:
    void parallelAgentsFinishedSignal();

private:

    ParallelismController();
    ParallelismController(ParallelismController const&);
    ~ParallelismController();

    QThread* main_thread;

    QList<QThread*> available_threads; // One per cpu

};

#endif // PARALLELISMCONTROLLER_H
