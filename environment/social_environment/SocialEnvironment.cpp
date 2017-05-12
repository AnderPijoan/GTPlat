#include <QDebug>

#include "SocialEnvironment.h"
#include "environment/Environment.h"

SocialEnvironment::SocialEnvironment(Environment *parent) : QObject(parent){
}

SocialEnvironment::~SocialEnvironment(){
}

/**********************************************************************
 METHODS
**********************************************************************/

bool SocialEnvironment::addAgent(Agent *agent){
    Q_UNUSED(agent);
    // Connect with social environment to send-receive messages
    //SocialEnvironment::connect( agent , SIGNAL( sendMessage(Message*) ) , this, SLOT( transportMessage(Message*) ) );

    return true;
}

bool SocialEnvironment::removeAgent(Agent *agent){
    Q_UNUSED(agent);
    return true;
}
