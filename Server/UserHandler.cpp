#include "UserHandler.h"

//UserHandler::UserHandler(){}

void clear_queue( std::queue<Message> &q )
{
   std::queue<Message> empty;
   std::swap( q, empty );
}

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

void UserHandler::initialize_thread(Message msg,bool is_server,std::promise<bool>& p){
    //messages_vector.clear();
    clear_queue(messages_vector);
    messages_vector.push(msg);
    isServer = is_server;
    window_counter = 0;
    refresh_keep_alive();
    run_keep_alive_check();
    
    UDPPacket packet(msg.get_complete_data());
    packets_handler.set_total_msg_size(packet.get_total_msg_size());
    
    loop();
    if ( ! is_server )
        p.set_value(true);
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
                     action_when_user_reaches_timeout();
                     printf("Peer computer %s:%d is disconnected.\n",
                                        inet_ntoa(destination.sin_addr),
                                        ntohs(destination.sin_port) );
                     keep_alive_periodic_checker.destroy(tid);
			packets_received_within_a_window.clear();
                 }
                 else if ( result > difference ){
                    ACKCommand ack;
                    if ( ! packets_received_within_a_window.empty() ) {
                        Message msg = ack.construct_packet(&packets_received_within_a_window);
                        //if ( msg.get_message_size() > 1)
                        Socket::UDPsend(sock_fd,&msg,destination,UPD_ENUM_COMMANDS::ACK);
                        printf("Periodic check:%s\n",msg.get_complete_data());
                    }
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
    messages_vector.push(msg);
}

bool UserHandler::is_packet_already_received(unsigned int packet_ID){
    std::vector<unsigned int>::iterator IntIterator;
    IntIterator = std::find(packets_received_within_a_window.begin(), 
                            packets_received_within_a_window.end(), 
                            packet_ID);
    return IntIterator != packets_received_within_a_window.end();
}

void UserHandler::loop(){
    
    while( true ){
        while ( messages_vector.empty() );
        
        Message msg = messages_vector.front();
        //printf("in USERHANDLER:\n");
        //msg.debug_print_msg();
        
        UDPPacket packet(messages_vector.back().get_complete_data());
        
        unsigned int packet_id = packet.get_remaining_packets();
        
        if ( is_packet_already_received(packet_id) ){
            messages_vector.pop();
            refresh_keep_alive();
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
        
        messages_vector.pop();
        
        if ( packets_handler.is_full_message_received() )
            break;
        
        refresh_keep_alive();
    }
    
    Message _msg(packets_handler.get_whole_received_data());
    
    message_notification(_msg);
    
    clear_queue(messages_vector);
    keep_alive_periodic_checker.destroy(tid);
    
    if ( isServer )
        server_Reaction_upon_success(_msg);
    
    
}


void UserHandler::handle_Acks(char *packet_bytes,SocketAddress destination,int sock){
    std::set<unsigned int> result;
    char packet[MAX_UDP_DATA_PACKET];
    ACKCommand ack;
    test_sock.received_packets = ack.parse_packet(packet_bytes);
    std::set_difference( test_sock.sent_packets.begin(), test_sock.sent_packets.end(), test_sock.received_packets.begin(), test_sock.received_packets.end(), std::inserter(result, result.begin()));
    printf("ACK responses are sent of the following messages: ");
    for( auto& packet_id : result){
        unsigned int size;
        bzero(packet, MAX_UDP_DATA_PACKET);
        test_sock.packetsHandler->get_specific_packet(packet,size,packet_id);
        Socket::raw_UDPsent(sock, packet, size, destination);
        printf("%d;", packet_id );
    }
    printf("\n");
}


std::string UserHandler::get_whole_data(){
    return packets_handler.get_whole_received_data();
}

void UserHandler::refresh_keep_alive(){
    keep_alive = std::chrono::system_clock::now();
}


