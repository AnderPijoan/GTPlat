#ifndef IMPORTER_H
#define IMPORTER_H

#include <QObject>

class Importer : public QObject
{
    Q_OBJECT
public:
    explicit Importer(QString importer_path , QObject *parent = 0);

protected:
    QString importer_path; // Can be a file or a directory depending on the importer
    QStringList extensions; // Extensions that can be imported
};

#endif // IMPORTER_H
