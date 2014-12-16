#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <future>
#include <chrono>

#include "../Server/UserHandler.h"
#include "../UDP/Socket.h"

class Client: public Socket {

public: 

	Client(); 
	status start_client(Message *message, Message *reply, char *recepient, unsigned short port=0);
	virtual status DoOperation (Message *message, Message *reply, int s, SocketAddress serverSA) = 0;
        void handle_Acks(char *packet_bytes,SocketAddress destination,int sock);
        
protected:
            
        void run_keep_alive_check();
        void refresh_keep_alive();
        std::promise<bool> p;
        std::future<bool> future_thread = p.get_future();
        
        std::chrono::system_clock::time_point keep_alive;
        Timer keep_alive_periodic_checker;
        Timer::timer_id tid;
        

};




#endif 
