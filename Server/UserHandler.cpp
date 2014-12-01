#include "UserHandler.h"

//UserHandler::UserHandler(){}


UserHandler::UserHandler(const char *client_ip,int port,SocketAddress sck_ad,int sock) : 
port(port), sock_fd(sock), destination(sck_ad)  {
    memcpy(connected_ip,client_ip,IPv4_SIZE);
    
}

UserHandler::UserHandler(const UserHandler& rhs){
    keep_alive = rhs.keep_alive;
    messages_vector = rhs.messages_vector;
    memcpy(connected_ip,rhs.connected_ip,IPv4_SIZE);
    port = rhs.port; 
    sock_fd = rhs.sock_fd;
    destination = rhs.destination;
    isServer = rhs.isServer;
    //server_instance = rhs.server_instance;
    //UDPPacketsHandler packets_handler;
}

UserHandler::~UserHandler() {
}

void UserHandler::initialize_thread(Message msg,bool is_server){
    //msg.debug_print_msg();
    messages_vector.clear();
    messages_vector.push_back(msg);
    isServer = is_server;
    window_counter = 0;
    refresh_keep_alive();
    run_keep_alive_check();
    
    UDPPacket packet(msg.get_complete_data());
    packets_handler.set_total_msg_size(packet.get_total_msg_size());
    
    loop();
}

// the Check occurs every one second
void UserHandler::run_keep_alive_check(){
    tid = keep_alive_periodic_checker.create(0, 1000,
             [&]() {
                 auto current_time = std::chrono::system_clock::now();
                 std::chrono::seconds sec(KEEP_ALIVE_CONSTANT);
                 std::chrono::seconds sec2(TIMEOUT_OF_REPLY);
                 auto difference = std::chrono::duration_cast<std::chrono::seconds>(sec);
                 auto client_death_difference = std::chrono::duration_cast<std::chrono::seconds>(sec2);
                 auto result = current_time - keep_alive ;
                 if ( result > client_death_difference ){
                     // call destructor
                     printf("Peer computer %s:%d is disconnected.\n",
                                        inet_ntoa(destination.sin_addr),
                                        ntohs(destination.sin_port) );
                     keep_alive_periodic_checker.destroy(tid);
                 }
                 else if ( result > difference ){
                    ACKCommand ack;
                    Message msg = ack.construct_packet(&packets_received_within_a_window);
                    if ( msg.get_message_size() != 0)
                        Socket::UDPsend(sock_fd,&msg,destination,UPD_ENUM_COMMANDS::ACK);
                    
                    printf("Periodic check:%s",msg.get_complete_data());
                    //msg.debug_print_msg();
                    printf("\n");
                    //window_counter = 0;
                    //packets_received_within_a_window.clear();
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
    //condition.notify_one();
}

bool UserHandler::is_packet_already_received(unsigned int packet_ID){
    std::vector<unsigned int>::iterator IntIterator;
    IntIterator = std::find(packets_received_within_a_window.begin(), 
                            packets_received_within_a_window.end(), 
                            packet_ID);
    return IntIterator != packets_received_within_a_window.end();
}

void UserHandler::loop(){
    //std::unique_lock<std::mutex> lock(queue_mutex);
    
    //condition.wait(lock);
    while( true ){
        //std::lock_guard<std::mutex> lock(queue_mutex);
        while ( messages_vector.empty() );
        
        Message msg = messages_vector.back();
        //printf("in USERHANDLER:\n");
        //msg.debug_print_msg();
        
        UDPPacket packet(messages_vector.back().get_complete_data());
        
        unsigned int packet_id = packet.get_remaining_packets();
        
        if ( is_packet_already_received(packet_id) ){
            messages_vector.pop_back();
            //refresh_keep_alive();
            continue;
        }
        
        packets_handler.add_UDPPacket( packet );

        packets_received_within_a_window.push_back(packet.get_remaining_packets());
        
        if ( ( ++window_counter % packet.get_window_size() == 0) ) {
            // send acknowledgment
            ACKCommand ack;
            Message msg = ack.construct_packet(&packets_received_within_a_window);
            Socket::UDPsend(sock_fd,&msg,destination,UPD_ENUM_COMMANDS::ACK_SUCCESS);
            packets_received_within_a_window.clear();
            printf("* Acknowledgment of %d packets was sent *\n",window_counter);
            window_counter = 0;
        }
        // Conditions based on the UDP Packet
        
        messages_vector.pop_back();
        
        if ( packets_handler.is_full_message_received() )
            break;
        
        refresh_keep_alive();
        //condition.wait(lock);
    }
    
    Message _msg(packets_handler.get_whole_received_data());
    
    //std::string ip = std::string(inet_ntoa(destination.sin_addr));
    //std::string port = std::to_string(ntohs(destination.sin_port));
    printf("Message was fully received from ( %s:%d ) : \n\tLength: %d bytes\n\tContent: %s\n\n",
                        inet_ntoa(destination.sin_addr),
                        ntohs(destination.sin_port),
                        _msg.get_message_size(),
                        _msg.get_string().c_str() );
    
    keep_alive_periodic_checker.destroy(tid);
    //if ( isServer )
    //    Socket::UDPsend_ACK_support(sock_fd, &_msg, destination,UPD_ENUM_COMMANDS::TRANSMIT_DATA);
}


std::string UserHandler::get_whole_data(){
    return packets_handler.get_whole_received_data();
}

void UserHandler::refresh_keep_alive(){
    keep_alive = std::chrono::system_clock::now();
}


