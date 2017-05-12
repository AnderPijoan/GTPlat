#include <QFile>
#include <QImage>
#include <QList>
#include <QBuffer>
#include <QJsonDocument>
#include "HttpRequestHandler.h"

#include "agents/Agent.h"
#include "utils/exporters/debugger/Debugger.h"
#include "environment/Environment.h"

HttpRequestHandler::HttpRequestHandler(QHttpRequest* req, QHttpResponse* res, Environment* environment) : QObject(req) {

    Debugger::debug( this , Q_FUNC_INFO , QString("Request entered : [%1] %2").arg( req->method() ).arg( req->url().toString() ) );
    this->environment = environment;

    // Append chunks of data into uniform body.
    req->onData([this](const QByteArray& chunk) {
        this->body.append(chunk);
    });

    // When all the incoming data are gathered, send some response to client.
    req->onEnd([this, req, res]() {
        this->handleHttpRequest( req , res );
    });
}

HttpRequestHandler::~HttpRequestHandler(){
}

/**********************************************************************
 METHODS
**********************************************************************/


void HttpRequestHandler::handleHttpRequest(QHttpRequest* req, QHttpResponse* res){

    // INDEX
    {
        QRegularExpressionMatch match = QRegularExpression("^/$").match( req->url().url() );
        if( match.hasMatch() ){ this->HTTP_Home( req , res , match ); return; }
    }

    // STATIC
    {
        QRegularExpressionMatch match = QRegularExpression("^/static/(?<file>[a-z0-9/.-]+)$").match( req->url().url() );
        if( match.hasMatch() ){ this->HTTP_Static( req , res , match ); return; }
    }

    // ENVIRONMENT
    {
        QRegularExpressionMatch match = QRegularExpression("^/environment$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_GET ){ this->HTTP_GetEnvironment( req , res , match ); return; }
        if( match.hasMatch() && req->method() == qhttp::EHTTP_PUT ){ this->HTTP_UpdateEnvironment( req , res , match ); return; }
    }
    {
        QRegularExpressionMatch match = QRegularExpression("^/environment/run$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_POST ){ this->HTTP_RunEnvironment( req , res , match ); return; }
    }
    {
        QRegularExpressionMatch match = QRegularExpression("^/environment/stop$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_POST ){ this->HTTP_StopEnvironment( req , res , match ); return; }
    }

    // AGENTS
    {
        QRegularExpressionMatch match = QRegularExpression("^/agents/(?<class_name>[A-Za-z0-9]+)/from/(?<from>[0-9]+)/limit/(?<limit>[0-9]+)$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_GET ){ this->HTTP_GetAll( req , res , match ); return; }
    }
    {
        QRegularExpressionMatch match = QRegularExpression("^/agents/(?<class_name>[A-Za-z0-9]+)/run$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_POST ){ this->HTTP_RunAll( req , res , match ); return; }
    }
    {
        QRegularExpressionMatch match = QRegularExpression("^/agents/(?<class_name>[A-Za-z0-9]+)/stop$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_POST ){ this->HTTP_StopAll( req , res , match ); return; }
    }
    {
        QRegularExpressionMatch match = QRegularExpression("^/agent/(?<class_name>[A-Za-z0-9]+)$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_POST ){ this->HTTP_CreateOne( req , res , match ); return; }
    }
    {
        QRegularExpressionMatch match = QRegularExpression("^/agent/(?<class_name>[A-Za-z0-9]+)/(?<id>[a-z0-9]+)$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_GET ){ this->HTTP_GetOne( req , res , match ); return; }
        if( match.hasMatch() && req->method() == qhttp::EHTTP_PUT ){ this->HTTP_UpdateOne( req , res , match ); return; }
        if( match.hasMatch() && req->method() == qhttp::EHTTP_DELETE ){ this->HTTP_DeleteOne( req , res , match ); return; }
    }
    {
        QRegularExpressionMatch match = QRegularExpression("^/agent/(?<class_name>[A-Za-z0-9]+)/(?<id>[a-z0-9]+)/run$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_POST ){ this->HTTP_RunOne( req , res , match ); return; }
    }
    {
        QRegularExpressionMatch match = QRegularExpression("^/agent/(?<class_name>[A-Za-z0-9]+)/(?<id>[a-z0-9]+)/stop$").match( req->url().url() );
        if( match.hasMatch() && req->method() == qhttp::EHTTP_POST ){ this->HTTP_StopOne( req , res , match ); return; }
    }

}


/**********************************************************************
 SERVER API
**********************************************************************/

void HttpRequestHandler::HTTP_Home(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match , QJsonObject body ){
    Q_UNUSED(req);
    Q_UNUSED(match);
    Q_UNUSED(body);

    // Read index file
    try{
        QFile file("../web/index.html");
        QByteArray filecontent = "";
        if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)){
            res->end();
        } else {
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                filecontent += in.readLine();
            }
            file.close();
            res->setStatusCode( qhttp::ESTATUS_OK );
            res->addHeader("Content-Length", QByteArray::number( filecontent.size() ) );
            res->write( filecontent );
        }
    } catch (...){}
    res->end();
}

void HttpRequestHandler::HTTP_Static(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);

    // Read the file
    try{
        QFile file("../web/" + match.captured("file"));
        QByteArray filecontent = "";
        if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)){
            res->end();
        } else {

            // If it is an image
            if( file.fileName().endsWith("PNG" , Qt::CaseInsensitive ) ||  file.fileName().endsWith("JPG" , Qt::CaseInsensitive )  ){

                QImage image = QImage( file.fileName() );

                // Transform QImage into QByteArray
                QByteArray filecontent;
                QBuffer buffer(&filecontent);
                buffer.open(QIODevice::WriteOnly);
                image.save( &buffer , "PNG" );

                res->setStatusCode( qhttp::ESTATUS_OK );
                res->addHeader("Content-Length", QByteArray::number( filecontent.size() ) );
                res->end(filecontent);

            } else {

                // Not an image
                QTextStream in(&file);
                while ( !in.atEnd() ) {
                    filecontent += in.readLine() + "\n";
                }
                file.close();
                res->setStatusCode( qhttp::ESTATUS_OK );
                res->addHeader("Content-Length", QByteArray::number( filecontent.size() ) );
                res->write( filecontent );
            }
        }
    } catch (...){}
    res->end();
}

/**********************************************************************
 ENVIRONMENT API
**********************************************************************/


void HttpRequestHandler::HTTP_GetEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        this->HTTP_WriteResponse( res , this->environment , "CREATE" );
    } catch(...){}
    res->end();
}

void HttpRequestHandler::HTTP_UpdateEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    try{
        this->environment->fromJSON( body );
        this->HTTP_WriteResponse( res , this->environment , "UPDATE" );
    } catch(...){}
    res->end();
}

void HttpRequestHandler::HTTP_RunEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        this->environment->runEnvironment();
        this->HTTP_WriteResponse( res , this->environment , "UPDATE" );
    } catch(...){}
    res->end();
}

void HttpRequestHandler::HTTP_StopEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        this->environment->stopEnvironment();
        this->HTTP_WriteResponse( res , this->environment , "UPDATE" );
    } catch(...){}
    res->end();
}

/**********************************************************************
 AGENTS API
**********************************************************************/

void HttpRequestHandler::HTTP_GetAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        if( this->environment->getClasses().contains( match.captured("class_name")) ){

            QList<Agent*> agents = this->environment->getByClass( match.captured("class_name") );
            QList<Agent*> paginated_agents = agents.mid( match.captured("from").toInt() , match.captured("limit").toInt() );

            this->HTTP_WriteResponse( res , paginated_agents , "CREATE" , agents.size() );
        }
    } catch(...){}
    res->end();
}

void HttpRequestHandler::HTTP_RunAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{

        if( this->environment->getClasses().contains( match.captured("class_name")) ){
            QList<Agent*> agents = this->environment->getByClass( match.captured("class_name") );
            for(int i = 0; i < agents.size() ; i++){
                Agent* agent = agents.at(i);
                this->environment->runAgent( agent );
            }
            this->HTTP_WriteResponse( res , QList<Agent*>() , "UPDATE" , agents.size() );
        }
    } catch(...){}
    res->end();
}

void HttpRequestHandler::HTTP_StopAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{

        if( this->environment->getClasses().contains( match.captured("class_name")) ){
            QList<Agent*> agents = this->environment->getByClass( match.captured("class_name") );
            for(int i = 0; i < agents.size() ; i++){
                Agent* agent = agents.at(i);
                this->environment->stopAgent( agent );
            }
            this->HTTP_WriteResponse( res , QList<Agent*>() , "UPDATE" , agents.size() );
        }
    } catch(...){}
    res->end();
}

void HttpRequestHandler::HTTP_CreateOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(match);
    Q_UNUSED(body);
    try{

    } catch(...){}
    res->end();
}

void HttpRequestHandler::HTTP_GetOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        Agent* agent = this->environment->getByClassAndId( match.captured("class_name"), match.captured("id").toInt());
        if( agent ){
            this->HTTP_WriteResponse( res , agent , "CREATE" );
        }
    } catch(...){}
    res->end();
}

void HttpRequestHandler::HTTP_UpdateOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    try{
        Agent* agent = this->environment->getByClassAndId( match.captured("class_name"), match.captured("id").toInt());
        if(agent){
            agent->fromJSON( body );
            this->HTTP_WriteResponse( res , agent , "UPDATE" );
        }
    } catch(...){}
    res->end();
}

void HttpRequestHandler::HTTP_RunOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        Agent* agent = this->environment->getByClassAndId( match.captured("class_name"), match.captured("id").toInt() );
        if( agent ){
            this->environment->runAgent( agent );
            this->HTTP_WriteResponse( res , agent , "UPDATE" );
        }
    } catch(...){}
    res->end();
}

void HttpRequestHandler::HTTP_StopOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        Agent* agent = this->environment->getByClassAndId( match.captured("class_name"), match.captured("id").toInt());
        if(agent){
            this->environment->stopAgent( agent );

            this->HTTP_WriteResponse( res , agent , "UPDATE" );
        }
    } catch(...){}
    res->end();
}

void HttpRequestHandler::HTTP_DeleteOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        Agent* agent = this->environment->getByClassAndId( match.captured("class_name"), match.captured("id").toInt());
        if (agent){
            this->environment->stopAgent( agent );
            this->environment->removeAgent( agent );
            this->HTTP_WriteResponse( res , agent , "DELETE" );
            agent->deleteLater();
        }

    } catch(...){ }
    res->end();
}


/**********************************************************************
 PRIVATES
**********************************************************************/

void HttpRequestHandler::HTTP_WriteResponse(QHttpResponse *res, Agent *agent , QString operation){

    res->setStatusCode( qhttp::ESTATUS_OK );
    res->addHeader("Content-Type", "application/json");

    QVariantMap json;
    json.insert("operation", operation.toUpper() );
    json.insert("data", agent->toJSON().toVariantMap());
    res->write( QJsonDocument::fromVariant( json ).toJson() );
}

void HttpRequestHandler::HTTP_WriteResponse(QHttpResponse *res, QList<Agent*> agents , QString operation , int total_count){

    res->setStatusCode( qhttp::ESTATUS_OK );
    res->addHeader("Content-Type", "application/json");

    QVariantMap json;
    json.insert("operation", operation.toUpper() );
    json.insert("count", total_count);
    QJsonArray json_agents;
    foreach(Agent* a , agents){
        json_agents.append( a->toMiniJSON() );
    }
    json.insert("data", json_agents.toVariantList());

    res->write( QJsonDocument::fromVariant( json ).toJson() );
}
