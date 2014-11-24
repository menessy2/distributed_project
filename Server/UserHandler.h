#ifndef USERHANDLER_H
#define	USERHANDLER_H

#define IPv4_SIZE   4       

#include <thread>
#include <mutex>
#include <condition_variable>
#include <string.h>
#include <iostream>

#include "../UDP/UDPPacket.h"

class UserHandler  {
public:
    UserHandler();
    UserHandler(const char *client_ip,int port);
    UserHandler(const UserHandler& rhs);
    void initialize_thread(const char *msg);
    void notify_user_about_incomming_message(const char *msg);
    virtual ~UserHandler();
    void set_port(int _port);
    void set_ip(char *ip_address);
    char *get_ip();
    int get_port();
    void loop();
    
private:
    int keep_alive;
    UDPPacketsHandler packets_handler;
    std::vector<std::string> messages_vector;
    
    std::mutex queue_mutex;
    std::condition_variable condition;
    char connected_ip[IPv4_SIZE]; 
    int port; 
    
};

#endif	/* USERHANDLER_H */

