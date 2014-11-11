#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include "Socket.h"
#include "Message.h"

class Server: public Socket {

public: 
	Server(unsigned short port=0); 
	status receive_from_clients (Message *received_message);
	status GetRequest (Message *callMessage, int s, SocketAddress *clientSA);
	status SendReply  (Message *replyMessage, int s, SocketAddress clientSA);

private:
	void makeReceiverSA(SocketAddress *sa, unsigned short port=0);
        Socket *mysock;
};

#endif
