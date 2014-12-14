#ifndef ECHOUSERHANDLER_H
#define	ECHOUSERHANDLER_H

#include "UserHandler.h"

class EchoUserHandler : public UserHandler {
    
public:
    
    EchoUserHandler(const char*, uint16_t, SocketAddress&, int&);
    virtual void message_notification(Message& msg);
    virtual void server_Reaction_upon_success(Message& msg);
    virtual void action_when_user_reaches_timeout();
    virtual void client_Reaction_upon_success(Message& msg);

};

#endif	/* ECHOUSERHANDLER_H */

