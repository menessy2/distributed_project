#include "UserHandler.h"

UserHandler::UserHandler(char *client_ip,int port) : std::thread(this()), port(port) {
    memcpy(connected_ip,client_ip,IPv4_SIZE);
    
}


UserHandler::~UserHandler() {
    this->join();
}


char *UserHandler::get_ip(){
    return connected_ip;
}

int UserHandler::get_port(){
    return port;
}

void UserHandler::notify_user_about_incomming_message(char *msg){
    messages_vector.push_back(std::string(msg));
    condition.notify_one();
}

void UserHandler::operator()(){
    
    while( ! packets_handler.is_full_message_received() ){
        
        std::unique_lock<std::mutex> lock(queue_mutex);
        packets_handler.parse_UDPPacket(messages_vector.pop_back());
        
        condition.wait(lock);
    }
    
}


