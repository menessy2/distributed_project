/*
MAX UDP PACKET IN IPv6 ( Jumbogram ) 65,536 and 4,294,967,295 
MAX UDP PACKET IN IPv4 65507 
*/

#ifndef SOCKET_H
#define SOCKET_H


#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <algorithm>
#include <set>
#include <mutex>
#include <condition_variable>

#include "../Payload/Message.h"
#include "Commands/ACKCommand.h"
#include "UDPPacket.h"

class Socket {

public:
	Socket();
	static status UDPsend(int s, Message *m, SocketAddress destination,UPD_ENUM_COMMANDS cmd=UPD_ENUM_COMMANDS::TRANSMIT_DATA);
        status UDPsend_ACK_support(int s, Message message, SocketAddress destination,UPD_ENUM_COMMANDS cmd);
        status UDPsend_ACK_support2(int s, Message message, SocketAddress destination,UPD_ENUM_COMMANDS cmd,std::string &session);
        status UDPsend_ACK_support_with_fd(int s, const char *filename, SocketAddress destination,UPD_ENUM_COMMANDS cmd,std::string &session);
	static status UDPreceive(int s, Message *m, SocketAddress *origin);
        static status raw_UDPsent(int s, char *packet, int size, SocketAddress destination);
        
        std::condition_variable condition;
        
        // ONLY TO BE USED IN "UDPsend_ACK_support"
        std::mutex queue_mutex;
        std::set<unsigned int> sent_packets,received_packets;
        UDPPacketsHandler *packetsHandler;
        
protected:
    
	int s;                              // used by the client
	SocketAddress destinationSA;        // used by the client
        
        int sockfd;                         // used by the server
        SocketAddress servaddr;             // used by the server
	
	void makeLocalSA(SocketAddress *sa, int port=-1);
	void makeDestSA(SocketAddress *sa, char *hostname, unsigned short port);
        
        
};

#endif


