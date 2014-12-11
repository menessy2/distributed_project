
#include "Socket.h"

Socket::Socket(){ }

status Socket::UDPsend(int           s,
                       Message *     message,
                       SocketAddress destination,
                        UPD_ENUM_COMMANDS cmd)
{
    int n, accumulative = 0;
    char packet[MAX_UDP_DATA_PACKET+1];
    unsigned int actual_packet_size;
    
    UDPPacketsHandler packetsHandler(message,cmd);      // make a new sequence number : to be changed
    
    while ( ! packetsHandler.is_transmission_reached_to_end() )
    {
        bzero(packet, MAX_UDP_DATA_PACKET);
        packetsHandler.get_next_packet(packet,actual_packet_size);
        
        if ((n = sendto(s, packet, actual_packet_size, 0, (struct sockaddr*) &destination,
                        sizeof(SocketAddress))) < 0)
        {
            perror("Send failed\n");
            return status::BAD;
        }
        accumulative += n;
    }

    return status::OK;
}


status Socket::UDPsend_ACK_support_with_fd(int s, const char *filename, \
                                    SocketAddress destination, \
                                    UPD_ENUM_COMMANDS cmd,std::string &session){
    
    int n;
    char packet[MAX_UDP_DATA_PACKET+1];
    unsigned int actual_packet_size;
    unsigned int window_counter = 0;
    
    packetsHandler = new UDPPacketsHandler(filename,cmd);      // make a new sequence number : to be changed
    packetsHandler->set_session(session);
    
    std::unique_lock<std::mutex> lock(queue_mutex);
    
    while ( ! packetsHandler->is_transmission_reached_to_end() )
    {
        do {
            bzero(packet, MAX_UDP_DATA_PACKET);
            packetsHandler->get_next_packet(packet,actual_packet_size); 

            if ((n = sendto(s, packet, actual_packet_size, 0, (struct sockaddr*) &destination,
                            sizeof(SocketAddress))) < 0)
            {
                perror("Send failed\n");
                return status::BAD;
            }
            
            UDPPacket udp_packet( packet );
            sent_packets.insert(udp_packet.get_remaining_packets());
            
            if ( ( ++window_counter % udp_packet.get_window_size() ) == 0 )
                break;
            
        } while ( true );
        
        // handle acks and
        // wait for ack success
        
        condition.wait(lock);
        sent_packets.clear();
        received_packets.clear();
        
    }
    
    printf("\nMessage was sent successfully:  %d bytes\n", packetsHandler->get_total_message_size() );

    return status::OK;
    

}

status Socket::UDPsend_ACK_support2(int s, Message message, \
        SocketAddress destination,UPD_ENUM_COMMANDS cmd,\
        std::string &session){

    int n;
    char packet[MAX_UDP_DATA_PACKET+1];
    unsigned int actual_packet_size;
    unsigned int window_counter = 0;
    
    packetsHandler = new UDPPacketsHandler(&message,cmd);      // make a new sequence number : to be changed
    packetsHandler->set_session(session);
    
    std::unique_lock<std::mutex> lock(queue_mutex);
    
    while ( ! packetsHandler->is_transmission_reached_to_end() )
    {
        do {
            bzero(packet, MAX_UDP_DATA_PACKET);
            packetsHandler->get_next_packet(packet,actual_packet_size);

            if ((n = sendto(s, packet, actual_packet_size, 0, (struct sockaddr*) &destination,
                            sizeof(SocketAddress))) < 0)
            {
                perror("Send failed\n");
                return status::BAD;
            }
            
            UDPPacket udp_packet( packet );
            sent_packets.insert(udp_packet.get_remaining_packets());
            
            if ( ( ++window_counter % udp_packet.get_window_size() ) == 0 )
                break;
            
        } while ( true );
        
        // handle acks and
        // wait for ack success
        
        condition.wait(lock);
        sent_packets.clear();
        received_packets.clear();
        
    }
    
    printf("\nMessage was sent successfully:  %d bytes\n", message.get_message_size());

    return status::OK;

}


status Socket::UDPsend_ACK_support(int s, 
                                    Message message, 
                                    SocketAddress destination,
                                    UPD_ENUM_COMMANDS cmd){
    int n;
    char packet[MAX_UDP_DATA_PACKET+1];
    unsigned int actual_packet_size;
    unsigned int window_counter = 0;
    
    packetsHandler = new UDPPacketsHandler(&message,cmd);      // make a new sequence number : to be changed
    
    std::unique_lock<std::mutex> lock(queue_mutex);
    
    while ( ! packetsHandler->is_transmission_reached_to_end() )
    {
        do {
            bzero(packet, MAX_UDP_DATA_PACKET);
            packetsHandler->get_next_packet(packet,actual_packet_size);

            if ((n = sendto(s, packet, actual_packet_size, 0, (struct sockaddr*) &destination,
                            sizeof(SocketAddress))) < 0)
            {
                perror("Send failed\n");
                return status::BAD;
            }
            
            UDPPacket udp_packet( packet );
            sent_packets.insert(udp_packet.get_remaining_packets());
            
            if ( ( ++window_counter % udp_packet.get_window_size() ) == 0 )
                break;
            
        } while ( true );
        
        // handle acks and
        // wait for ack success
        
        condition.wait(lock);
        sent_packets.clear();
        received_packets.clear();
        
    }
    
    printf("\nMessage was sent successfully:  %d bytes\n", message.get_message_size());

    return status::OK;
}


status Socket::raw_UDPsent(int s, char *packet, int size, SocketAddress destination){
    int n;
    if ((n = sendto(s, packet, size, 0, (struct sockaddr*) &destination,
                       sizeof(SocketAddress))) < 0)
       {
           perror("Send failed\n");
           return status::BAD;
       }
    
    return status::OK;
}


status Socket::UDPreceive(int             s,
                          Message *       m,
                          SocketAddress * origin)
{
    int  n;
    char received_message[MAX_UDP_DATA_PACKET];
    origin -> sin_family = AF_INET;
    unsigned int length_received_msg = sizeof(SocketAddress);
    
    memset(received_message,0,MAX_UDP_DATA_PACKET);
    if ((n = recvfrom(s, received_message, MAX_UDP_DATA_PACKET, 0, 
                     (struct sockaddr*) origin, &length_received_msg))  < 0)
    {
        perror("Error on Receiving");
        return status::BAD;
    }
    else
    {
        received_message[n] = '\0';
    }
    
    m->set_string(received_message,n);
    return status::OK;
}

void Socket::makeLocalSA(SocketAddress * sa,
                         int             port)
{
    sa -> sin_family = AF_INET;
    sa -> sin_port   = htons(0);

    if (port == -1)
    {
        sa -> sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        sa -> sin_addr.s_addr = htonl(port);
    }
}

/* make a socket address for a destination whose machine and port are given as arguments */
void Socket::makeDestSA(SocketAddress * sa,
                        char *          hostname,
                        unsigned short  port)
{
    struct hostent *host;
    sa -> sin_family = AF_INET;
    if ((host = gethostbyname(hostname)) == NULL)
    {
        printf("Unknown host name\n");
        exit(-1);
    }

    sa -> sin_addr = *(struct in_addr*) (host -> h_addr);
    sa -> sin_port = htons(port);
}


