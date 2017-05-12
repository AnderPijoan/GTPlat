#include "NoReplyAPIDriver.h"

NoReplyAPIDriver::NoReplyAPIDriver(QObject *parent) : ASyncAPIDriver( parent ){

}

/**********************************************************************
 PRIVATE
**********************************************************************/

QNetworkReply* NoReplyAPIDriver::operation(QNetworkAccessManager::Operation operation, QUrl url, QMap<QString, QString> headers, QByteArray data, QByteArray custom_operation){
    QNetworkReply* reply = ASyncAPIDriver::operation( operation , url , headers , data , custom_operation );
    this->connect( reply , &QNetworkReply::finished , reply , &QNetworkReply::deleteLater );
    return 0;
}
