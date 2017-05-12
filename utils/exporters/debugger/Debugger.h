#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <QObject>

class Debugger : public QObject
{
    Q_OBJECT

public:
    static Debugger* getInstance();

    // SETTERS
    static void setDebugLevel(int level);

    // METHODS
    static void debug(const QObject *caller, QString function, QString message = QString()); // OP 1
    static void info(const QObject *caller, QString function, QString message = QString()); // OP 2
    static void warning(const QObject *caller, QString function, QString message = QString()); // OP 3
    static void critical(const QObject *caller, QString function, QString message = QString()); // OP 4

protected:
    static void operation(const QObject* caller,  QString function, QString message , int operation );

private:
    explicit Debugger();

    static int debug_show_level;
    static bool debug_to_log;
    static QStringList debug_allowed_classes;
};

#endif // DEBUGGER_H
