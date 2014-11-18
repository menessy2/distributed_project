#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include "Socket.h"
#include "Message.h"
#include "ThreadPool.h"


class Server: public Socket {

public: 
	Server(unsigned short port=0); 
        ~Server();
        int wait_and_handle_clients();
        void set_ThreadPool_size(size_t threads_size);
	void handle_client(Message * received_message);
	status GetRequest (Message *callMessage, int s, SocketAddress *clientSA);
	status SendReply  (Message *replyMessage, int s, SocketAddress clientSA);

private:
	void makeReceiverSA(SocketAddress *sa, unsigned short port=0);
        Socket *mysock;
        ThreadPool *pool;
};

#endif
