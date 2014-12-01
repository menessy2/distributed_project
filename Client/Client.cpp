
#include "Client.h"

Client::Client()
{
    printf("Welcome to CLIENT MODE:\n");
}

status Client::echo_reply_Message(Message *      message,
                                  Message *      reply,
                                  char *         recepient,
                                  unsigned short port)
{
    Socket::makeDestSA(&destinationSA, recepient, port);

    return DoOperation(message, reply, s, destinationSA);
}

void Client::handle_Acks(char *packet_bytes,SocketAddress destination,int sock){
    std::set<unsigned int> result;
    char packet[MAX_UDP_DATA_PACKET];
    ACKCommand ack;
    received_packets = ack.parse_packet(packet_bytes);
    std::set_difference( sent_packets.begin(), sent_packets.end(), received_packets.begin(), received_packets.end(), std::inserter(result, result.begin()));
    printf("ACK responses are sent of the following messages: ");
    for( auto& packet_id : result){
        unsigned int size;
        bzero(packet, MAX_UDP_DATA_PACKET);
        packetsHandler->get_specific_packet(packet,size,packet_id);
        Socket::raw_UDPsent(sock, packet, size, destination);
        printf("%d;", packet_id );
    }
    printf("\n");
}

status Client::DoOperation(Message *     message,
                           Message *     reply,
                           int           s,
                           SocketAddress serverSA)
{
    //ServerHandler handler;
    
    //status request_status = Socket::UDPsend_ACK_support(s, message, serverSA,UPD_ENUM_COMMANDS::TRANSMIT_DATA);
    std::thread *t = new std::thread(&Socket::UDPsend_ACK_support, this, s, *message, serverSA,UPD_ENUM_COMMANDS::TRANSMIT_DATA);
    //status reply_status   = Socket::UDPreceive(s, reply, &originSA);
    
    
    
    while ( true ) { 
        Message expected_Ack;
        Socket::UDPreceive(s, &expected_Ack, &serverSA);
    
        UDPPacket packet(expected_Ack.get_complete_data());
        
        switch( packet.get_command() ){
            case UPD_ENUM_COMMANDS::ACK:
                handle_Acks(expected_Ack.get_complete_data(),serverSA,s);
                break;
            case UPD_ENUM_COMMANDS::ACK_SUCCESS:
                condition.notify_one();   // continue transmission with the new bulk of the window 
                break;
        
        }
        
    }
        
    t->join();
    /*
    std::string ip = std::string(inet_ntoa(serverSA.sin_addr));
    UserHandler user = UserHandler(ip.c_str(),ntohs(serverSA.sin_port),serverSA,s);
    status reply_status   = Socket::UDPreceive(s, reply, &originSA);
    user.initialize_thread(*reply,false);
    
    printf("Whole message received from client: %s",user.get_whole_data().c_str());
    
    std::string data = user.get_whole_data();
    reply = &Message(data);
    */
    close(s);

    return status::OK;
}


