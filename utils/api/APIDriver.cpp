#include "APIDriver.h"
#include <QDebug>
#include <QNetworkProxyFactory>
#include <QNetworkProxy>

APIDriver::APIDriver(QObject *parent) : QObject( parent ){

    QNetworkProxyQuery npq( QUrl( "http://www.google.com" ) );
    QList<QNetworkProxy> proxies_list = QNetworkProxyFactory::systemProxyForQuery( npq );

    if( proxies_list.isEmpty() ){
        qWarning() << Q_FUNC_INFO << "No proxy found.";
    }

    this->access_manager = new QNetworkAccessManager( this );
    this->access_manager->setProxy( proxies_list.at(0) );
}

APIDriver::~APIDriver(){
    this->access_manager->deleteLater();
}

/**********************************************************************
 GETTERS
**********************************************************************/

unsigned int APIDriver::getCurrentRequests() const{
    return this->current_requests;
}

/**********************************************************************
 GET
**********************************************************************/

/**
 * WARNING! Caller gets ownership of QNetworkReply and must delete it
 * @brief APIDriver::syncGET
 * @param url
 * @return
 */
QNetworkReply* APIDriver::GET( QUrl url, QMap<QString, QString> headers ){
    return this->operation( QNetworkAccessManager::GetOperation , url , headers );
}

/**********************************************************************
 POST
**********************************************************************/

/**
   WARNING, Caller gets ownership of QNetworkReply and must delete it
 * @brief APIDriver::syncPOST
 * @param url
 * @return
 */
QNetworkReply* APIDriver::POST(QUrl url, QMap<QString, QString> headers, QByteArray data ){
    return this->operation( QNetworkAccessManager::PostOperation , url , headers , data );
}

/**********************************************************************
 PUT
**********************************************************************/

/**
   WARNING, Caller gets ownership of QNetworkReply and must delete it
 * @brief APIDriver::syncPATCH
 * @param url
 * @return
 */
QNetworkReply* APIDriver::PUT(QUrl url, QMap<QString, QString> headers, QByteArray data ){
    return this->operation( QNetworkAccessManager::PutOperation , url , headers , data );
}

/**********************************************************************
 PATCH
**********************************************************************/

/**
   WARNING, Caller gets ownership of QNetworkReply and must delete it
 * @brief APIDriver::syncPATCH
 * @param url
 * @return
 */
QNetworkReply* APIDriver::PATCH(QUrl url, QMap<QString, QString> headers, QByteArray data ){
    return this->operation( QNetworkAccessManager::CustomOperation , url , headers , data , "PATCH" );
}

/**********************************************************************
 DELETE
**********************************************************************/

/**
 * WARNING! Caller gets ownership of QNetworkReply and must delete it
 * @brief APIDriver::syncGET
 * @param url
 * @return
 */
QNetworkReply* APIDriver::DELETE( QUrl url, QMap<QString, QString> headers ){
    return this->operation( QNetworkAccessManager::DeleteOperation , url , headers );
}

/**********************************************************************
 PROTECTED
**********************************************************************/

QNetworkReply* APIDriver::operation(QNetworkAccessManager::Operation operation, QUrl url, QMap<QString, QString> headers, QByteArray data, QByteArray custom_operation){
    return this->operation( operation , url , headers );
}
