#include "UserHandler.h"

UserHandler::UserHandler(){}


UserHandler::UserHandler(const char *client_ip,int port,SocketAddress sck_ad,int sock,Server *server) : 
port(port), sock_fd(sock), destination(sck_ad), server_instance(server) {
    memcpy(connected_ip,client_ip,IPv4_SIZE);
    
}

UserHandler::UserHandler(const UserHandler& rhs){
    keep_alive = rhs.keep_alive;
    messages_vector = rhs.messages_vector;
    memcpy(connected_ip,rhs.connected_ip,IPv4_SIZE);
    port = rhs.port; 
    sock_fd = rhs.sock_fd;
    destination = rhs.destination;
    server_instance = rhs.server_instance;
    //UDPPacketsHandler packets_handler;
}

UserHandler::~UserHandler() {
}

void UserHandler::initialize_thread(const Message *msg){
    messages_vector.push_back(*msg);
    window_counter = 0;
    refresh_keep_alive();
    run_keep_alive_check();
    loop();
}

// the Check occurs every one second
void UserHandler::run_keep_alive_check(){
    Timer t;
    t.create(0, 1000,
             []() {
                 auto current_time = std::chrono::system_clock::now();
                 auto difference = std::chrono::duration_cast<std::chrono::seconds>(KEEP_ALIVE_CONSTANT);
                 if ( ( current_time - keep_alive ) > difference ){
                     
                 }
             });
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
    
    std::unique_lock<std::mutex> lock(queue_mutex);
    while( true ){
        
        Message msg = messages_vector.back();
        UDPPacket packet = packets_handler.parse_UDPPacket( msg.get_complete_data() );
        
        
        if ( ( ++window_counter % NUMBER_OF_CHUNCKS_TO_ACK == 0) ) {
            // send acknowledgment
            window_counter = 0;
            packets_received_within_a_window.clear();
        }
        // Conditions based on the UDP Packet
        
        messages_vector.pop_back();
        
        if ( packets_handler.is_full_message_received() )
            break;
        
        refresh_keep_alive();
        condition.wait(lock);
    }
    
    // server_instance->UDPsend(sock_fd,Message *message,destination,UPD_ENUM_COMMANDS::ACK);
    
    printf("Message was fully received: %s", packets_handler.get_data() );
}


void UserHandler::refresh_keep_alive(){
    keep_alive = std::chrono::system_clock::now();
}


