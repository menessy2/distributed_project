#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include "../UDP/Socket.h"

class ServerHandler{
public:
    ServerHandler(SocketAddress sck,int sock);
    void requests_dispatcher();                // alias listener
    void get_whole_data();
    void initialize_thread(const Message *msg);
    
private:
    std::mutex queue_mutex;
    std::condition_variable condition;
    SocketAddress sck_add;
    int sockfd;
};






#endif