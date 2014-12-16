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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <future>
#include <queue>

#include "../UDP/UDPPacket.h"
#include "../UDP/Socket.h"
#include "../UDP/Commands/ACKCommand.h"
#include "../Payload/Message.h"
#include "../Misc/timer.h"
#include "Server.h"

class Server;

void clear_queue( std::queue<Message> &q );

class UserHandler  {
public:
    
    UserHandler(const char *client_ip,int port,SocketAddress sck,int sock);
    UserHandler(const UserHandler& rhs);
    void initialize_thread(Message msg,bool is_server,std::promise<bool>& p);
    void notify_user_about_incomming_message(const Message msg);
    virtual ~UserHandler();
    void set_port(int _port);
    void set_ip(char *ip_address);
    char *get_ip();
    int get_port();
    virtual void loop();
    std::string get_whole_data();
    bool is_packet_already_received(unsigned int packet_ID);
    void handle_Acks(char *packet_bytes,SocketAddress destination,int sock);
    
    
    virtual void message_notification(Message& msg) = 0;
    virtual void server_Reaction_upon_success(Message& msg) = 0;
    virtual void client_Reaction_upon_success(Message& msg) = 0;
    virtual void action_when_user_reaches_timeout() = 0;
    
    std::queue<Message> messages_vector;
    
protected:
    std::chrono::system_clock::time_point keep_alive;
    unsigned int window_counter;
    UDPPacketsHandler packets_handler;
    
    
    std::mutex queue_mutex;
    std::condition_variable condition;
    char connected_ip[IPv4_SIZE]; 
    int port; 
    
    int sock_fd;
    SocketAddress destination;
    
    Timer keep_alive_periodic_checker;
    Timer::timer_id tid;
    
    bool isServer;
    Socket test_sock;               // socket for echoing the message
    
    // vector takes remaining packets as an element
    std::vector<unsigned int> packets_received_within_a_window;
    
    void refresh_keep_alive();
    void run_keep_alive_check();
    
};



#endif	/* USERHANDLER_H */

