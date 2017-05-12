#ifndef SYNCAPIDRIVER_H
#define SYNCAPIDRIVER_H

#include "utils/api/APIDriver.h"

class SyncAPIDriver : public APIDriver
{
    Q_OBJECT

public:
    SyncAPIDriver( QObject* parent = 0 );

protected:
    virtual QNetworkReply* operation( QNetworkAccessManager::Operation operation , QUrl url , QMap<QString, QString> headers = QMap<QString, QString>() , QByteArray data = QByteArray() , QByteArray custom_operation = QByteArray() );

};

#endif // SYNCAPIDRIVER_H
