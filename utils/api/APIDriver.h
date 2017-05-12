#ifndef APIDRIVER_H
#define APIDRIVER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QBuffer>

class APIDriver : public QObject
{
    Q_OBJECT
public:

    // GETTERS
    unsigned int getCurrentRequests() const;

    // GET
    virtual QNetworkReply* GET( QUrl url, QMap<QString, QString> headers = QMap<QString, QString>() );

    // POST
    virtual QNetworkReply* POST( QUrl url , QMap<QString, QString> headers = QMap<QString, QString>(), QByteArray data = QByteArray() );

    // PUT
    virtual QNetworkReply* PUT( QUrl url , QMap<QString, QString> headers = QMap<QString, QString>(), QByteArray data = QByteArray() );

    // PATCH
    virtual QNetworkReply* PATCH( QUrl url , QMap<QString, QString> headers = QMap<QString, QString>(), QByteArray data = QByteArray());

    // DELETE
    virtual QNetworkReply* DELETE( QUrl url, QMap<QString, QString> headers = QMap<QString, QString>() );

protected:
    virtual QNetworkReply* operation( QNetworkAccessManager::Operation operation , QUrl url , QMap<QString, QString> headers = QMap<QString, QString>() , QByteArray data = QByteArray() , QByteArray custom_operation = QByteArray() );

    APIDriver(QObject* parent = 0);
    ~APIDriver();

    QNetworkAccessManager* access_manager;
    double requests_timeout = 5; // IN SECONDS
    unsigned int current_requests = 0;

};

#endif // APIDRIVER_H
