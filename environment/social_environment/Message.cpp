#include "Message.h"

Message::Message(MessageConstants::message_types performative , QString conversation_id , Agent* sender , Agent* receiver , QObject *parent) : QObject(parent) , performative(performative) , conversation_id(conversation_id) , sender(sender) , receiver(receiver){
}

/**********************************************************************
 GETTERS
**********************************************************************/



/**********************************************************************
 SETTERS
**********************************************************************/
