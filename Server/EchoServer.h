#ifndef ECHOSERVER_H
#define	ECHOSERVER_H

#include "Server.h"
#include "EchoUserHandler.h"

class EchoServer : public Server {
    
public:
    
    EchoServer(int);
    virtual void dispatch_connection_to_UserHandler(Message *received_pkt,SocketAddress sck);

};

#endif	/* ECHOSERVER_H */

