#ifndef USERHANDLER_H
#define	USERHANDLER_H

#define IPv4_SIZE               4       // in bytes     


#include <thread>
#include <mutex>
#include <condition_variable>
#include <string.h>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

#include "../UDP/UDPPacket.h"
#include "../Payload/Message.h"
#include "../Misc/timer.h"

class UserHandler  {
public:
    UserHandler();
    UserHandler(const char *client_ip,int port,SocketAddress sck,int sock,Server *server);
    UserHandler(const UserHandler& rhs);
    void initialize_thread(const Message *msg);
    void notify_user_about_incomming_message(const Message &msg);
    virtual ~UserHandler();
    void set_port(int _port);
    void set_ip(char *ip_address);
    char *get_ip();
    int get_port();
    void loop();
    
    
private:
    std::chrono::system_clock::time_point keep_alive;
    unsigned int window_counter;
    UDPPacketsHandler packets_handler;
    std::vector<Message> messages_vector;
    
    std::mutex queue_mutex;
    std::condition_variable condition;
    char connected_ip[IPv4_SIZE]; 
    int port; 
    
    int sock_fd;
    SocketAddress destination;
    Server *server_instance;
    std::vector<unsigned int> packets_received_within_a_window;
    
    void refresh_keep_alive();
    void run_keep_alive_check();
    
};

#endif	/* USERHANDLER_H */

