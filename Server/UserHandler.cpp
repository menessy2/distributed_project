#include "UserHandler.h"

UserHandler::UserHandler(){}


UserHandler::UserHandler(const char *client_ip,int port) : port(port) {
    memcpy(connected_ip,client_ip,IPv4_SIZE);
    
}

void UserHandler::initialize_thread(const Message *msg){
    messages_vector.push_back(*msg);
    loop();
}


UserHandler::UserHandler(const UserHandler& rhs){
    keep_alive = rhs.keep_alive;
    messages_vector = rhs.messages_vector;
    memcpy(connected_ip,rhs.connected_ip,IPv4_SIZE);
    port = rhs.port; 
    //UDPPacketsHandler packets_handler;
}

UserHandler::~UserHandler() {
    //this->join();
}


char *UserHandler::get_ip(){
    return connected_ip;
}

int UserHandler::get_port(){
    return port;
}

void UserHandler::notify_user_about_incomming_message(const Message &msg){
    messages_vector.push_back(msg);
    condition.notify_one();
}

void UserHandler::loop(){
    
    while( true ){
        
        Message msg = messages_vector.back();
        std::unique_lock<std::mutex> lock(queue_mutex);
        packets_handler.parse_UDPPacket( msg.get_complete_data() );
        
        // Conditions based on the UDP Packet
        
        messages_vector.pop_back();
        
        if ( packets_handler.is_full_message_received() )
            break;
        condition.wait(lock);
    }
    printf("Message was fully received: %s", packets_handler.get_data() );
}



