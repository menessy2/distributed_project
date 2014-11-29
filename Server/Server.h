#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#include "../UDP/Socket.h"
#include "../Payload/Message.h"
#include "ThreadPool.h"
#include "UserHandler.h"

class Server: public Socket {

public: 
	Server(unsigned short port=0); 
        ~Server();
        int wait_and_handle_clients();
        void set_ThreadPool_size(size_t threads_size);
        void dispatch_connection_to_UserHandler(Message *received_pkt,SocketAddress sck);
	status GetRequest (Message *callMessage, int s, SocketAddress *clientSA);
	status SendReply  (Message *replyMessage, int s, SocketAddress clientSA);
        void doReaction(SocketAddress sck,int sockfd);

private:
	void makeReceiverSA(SocketAddress *sa, unsigned short port=0);
        Socket *mysock;
        //ThreadPool *outgoing_responses;
        ThreadPool *incomming_requests;
        std::map<std::string, UserHandler> user_handlers;
        std::mutex mutex_for_dispatcher;
};

#endif
