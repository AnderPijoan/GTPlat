#ifndef ASYNCAPIDRIVER_H
#define ASYNCAPIDRIVER_H

#include "utils/api/APIDriver.h"

class ASyncAPIDriver : public APIDriver
{
    Q_OBJECT

public:
    ASyncAPIDriver( QObject* parent = 0 );

    // GETTERS
    bool isFinished( QNetworkReply* unfinished_reply ) const;
    void removeReply( QNetworkReply* reply );

protected:
    virtual QNetworkReply* operation( QNetworkAccessManager::Operation operation , QUrl url , QMap<QString, QString> headers = QMap<QString, QString>() , QByteArray data = QByteArray() , QByteArray custom_operation = QByteArray() );

private:
    QMap<QNetworkReply* , bool> finished_replies;

};

#endif // ASYNCAPIDRIVER_H
