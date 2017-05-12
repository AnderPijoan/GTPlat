#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QStringList>

QT_FORWARD_DECLARE_CLASS(Agent)

class Logger : public QObject
{
    Q_OBJECT
public:

    // METHODS
    static void log(QVariant to_write, QString filename = "log");
    static void log(QStringList list, QString filename = "log");
    static void logAgent( Agent* agent , QString filename = "log");

private:
    // METHODS
    static QString getDomainPath(const QMetaObject agent_class);

    static QString logs_folder_name;
};

#endif // LOGGER_H

