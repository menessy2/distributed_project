#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



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
        void dispatch_connection_to_UserHandler(char *received_msg,SocketAddress sck);
	status GetRequest (Message *callMessage, int s, SocketAddress *clientSA);
	status SendReply  (Message *replyMessage, int s, SocketAddress clientSA);

private:
	void makeReceiverSA(SocketAddress *sa, unsigned short port=0);
        Socket *mysock;
        ThreadPool *pool;
        std::map<std::string, UserHandler> user_handlers;
};

#endif
