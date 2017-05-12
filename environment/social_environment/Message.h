#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QString>
#include <QDateTime>

#include "agents/Agent.h"
#include "environment/social_environment/MessageConstants.h"

QT_FORWARD_DECLARE_CLASS(Agent)

class Message : public QObject
{
    Q_OBJECT

public:
    Message(MessageConstants::message_types performative , QString conversation_id , Agent* sender , Agent* receiver , QObject* parent = 0);

    const MessageConstants::message_types performative;
    const QString conversation_id;
    const Agent* sender;
    const Agent* receiver;
    //const QString language;
    //const QString ontology;
    //const QString content;

};

#endif // MESSAGE_H
