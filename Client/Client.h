#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include "../UDP/Socket.h"

class Client: public Socket {

public: 

	Client(); 
	status echo_reply_Message (Message *message, Message *reply, char *recepient, unsigned short port=0);
	status DoOperation (Message *message, Message *reply, int s, SocketAddress serverSA);


};




#endif 
