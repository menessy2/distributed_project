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

#include "../Message.h"
#include "UDPPacket.h"

class Socket {

public:
	Socket();
	Socket(int); // port given as argument
	status UDPsend(int s, Message *m, SocketAddress destination);
	status UDPreceive(int s, Message *m, SocketAddress *origin);

protected:
	int s;
	SocketAddress originSA;
	SocketAddress destinationSA;
        int sockfd;
        SocketAddress servaddr,cliaddr;
        status STATUS;
	
	void makeLocalSA(SocketAddress *sa, int port=-1);
	void makeDestSA(SocketAddress *sa, char *hostname, unsigned short port);

};

#endif

