#include "UserHandler.h"

UserHandler::UserHandler(){}


UserHandler::UserHandler(const char *client_ip,int port,SocketAddress sck_ad,int sock) : 
port(port), sock_fd(sock), destination(sck_ad) /*, server_instance(server) */ {
    memcpy(connected_ip,client_ip,IPv4_SIZE);
    
}

UserHandler::UserHandler(const UserHandler& rhs){
    keep_alive = rhs.keep_alive;
    messages_vector = rhs.messages_vector;
    memcpy(connected_ip,rhs.connected_ip,IPv4_SIZE);
    port = rhs.port; 
    sock_fd = rhs.sock_fd;
    destination = rhs.destination;
    //server_instance = rhs.server_instance;
    //UDPPacketsHandler packets_handler;
}

UserHandler::~UserHandler() {
}

void UserHandler::initialize_thread(const Message *msg){
    messages_vector.push_back(*msg);
    window_counter = 0;
    refresh_keep_alive();
    run_keep_alive_check();
    
    UDPPacket packet(msg->get_complete_data());
    packets_handler.set_total_msg_size(packet.get_total_msg_size());
    
    loop();
}

// the Check occurs every one second
void UserHandler::run_keep_alive_check(){
    keep_alive_periodic_checker.create(0, 1000,
             [&]() {
                 auto current_time = std::chrono::system_clock::now();
                 std::chrono::seconds sec(KEEP_ALIVE_CONSTANT);
                 auto difference = std::chrono::duration_cast<std::chrono::seconds>(sec);
                 if ( ( current_time - keep_alive ) > difference ){
                    ACKCommand ack;
                    Message msg = ack.construct_packet(&packets_received_within_a_window);
                    Socket::UDPsend(sock_fd,&msg,destination,UPD_ENUM_COMMANDS::ACK);
                    window_counter = 0;
                    packets_received_within_a_window.clear();
                 }
             });
}


char *UserHandler::get_ip(){
    return connected_ip;
}

int UserHandler::get_port(){
    return port;
}

void UserHandler::notify_user_about_incomming_message(const Message msg){
    messages_vector.push_back(msg);
    condition.notify_one();
}

void UserHandler::loop(){
    std::unique_lock<std::mutex> lock(queue_mutex);
    //condition.wait(lock);
    while( ! messages_vector.empty() ){
        
        Message msg = messages_vector.back();
        UDPPacket packet = packets_handler.parse_UDPPacket( msg.get_complete_data() );
        
        packets_received_within_a_window.push_back(packet.get_remaining_packets());
        
        if ( ( ++window_counter % packet.get_window_size() == 0) ) {
            // send acknowledgment
            ACKCommand ack;
            Message msg = ack.construct_packet(&packets_received_within_a_window);
            Socket::UDPsend(sock_fd,&msg,destination,UPD_ENUM_COMMANDS::ACK_SUCCESS);
            window_counter = 0;
            packets_received_within_a_window.clear();
        }
        // Conditions based on the UDP Packet
        
        messages_vector.pop_back();
        
        if ( packets_handler.is_full_message_received() )
            break;
        
        refresh_keep_alive();
        //condition.wait(lock);
    }
    
    printf("Message was fully received: %s\n\n", packets_handler.get_whole_received_data().c_str() );
}


std::string UserHandler::get_whole_data(){
    return packets_handler.get_whole_received_data();
}

void UserHandler::refresh_keep_alive(){
    keep_alive = std::chrono::system_clock::now();
}


