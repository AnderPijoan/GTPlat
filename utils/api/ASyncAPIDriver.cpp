#include "ASyncAPIDriver.h"

#include "utils/exporters/debugger/Debugger.h"

ASyncAPIDriver::ASyncAPIDriver(QObject *parent) : APIDriver( parent ){
}

/**********************************************************************
 GETTERS
**********************************************************************/

bool ASyncAPIDriver::isFinished(QNetworkReply *unfinished_reply) const{
    return this->finished_replies.value( unfinished_reply , false );
}

void ASyncAPIDriver::removeReply(QNetworkReply *reply){
    this->finished_replies.remove( reply );
}

/**********************************************************************
 PRIVATE
**********************************************************************/

QNetworkReply* ASyncAPIDriver::operation(QNetworkAccessManager::Operation operation, QUrl url, QMap<QString, QString> headers, QByteArray data , QByteArray custom_operation ){

    Debugger::debug( this , Q_FUNC_INFO , QString("Performing operation %1 %2").arg( operation ).arg( url.toString() ) );

    // Request
    QNetworkRequest request(url);

    // Add request headers
    foreach ( const QString header, headers.keys() ){
        request.setRawHeader( header.toStdString().c_str(), headers.value( header ).toStdString().c_str() );
    }

    // Add data
    QBuffer buffer;
    buffer.open( ( QBuffer::ReadWrite ) );
    buffer.write( data );
    buffer.seek( 0 );

    QNetworkReply* unfinished_reply = 0;

    try {

        if( operation == QNetworkAccessManager::GetOperation ){ unfinished_reply = this->access_manager->get( request ); }
        if( operation == QNetworkAccessManager::PostOperation ){ unfinished_reply = this->access_manager->post( request , &buffer ); }
        if( operation == QNetworkAccessManager::PutOperation ){ unfinished_reply = this->access_manager->put( request , &buffer ); }
        if( operation == QNetworkAccessManager::CustomOperation ){ unfinished_reply = this->access_manager->sendCustomRequest( request , custom_operation , &buffer ); }
        if( operation == QNetworkAccessManager::DeleteOperation ){ unfinished_reply = this->access_manager->deleteResource( request ); }

        this->finished_replies.insert( unfinished_reply , false );
        this->current_requests++;

        connect( unfinished_reply, &QNetworkReply::finished , [unfinished_reply,this]{
                    this->finished_replies.insert( unfinished_reply , true );
                    this->current_requests--;
                });

    } catch(...){
        Debugger::warning( this , Q_FUNC_INFO , QString("Crashed request %1 %2").arg( operation ).arg( url.toString() ) );
    }

    return unfinished_reply; // Unfinished reply, receiver will need to ask if finished signal
}
