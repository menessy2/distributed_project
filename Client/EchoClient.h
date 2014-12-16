#ifndef ECHOCLIENT_H
#define	ECHOCLIENT_H

#include "Client.h"
#include "../Server/EchoUserHandler.h"

class EchoClient : public Client {
public:
    EchoClient();
    virtual status DoOperation (Message *message, Message *reply, int s, SocketAddress serverSA);
    

};

#endif	/* ECHOCLIENT_H */

