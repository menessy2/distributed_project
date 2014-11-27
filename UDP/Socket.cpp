
#include "Socket.h"

Socket::Socket()
{
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
    }
    bzero(&originSA, sizeof(SocketAddress));
    makeLocalSA(&originSA);
    if (bind(s, (struct sockaddr*)&originSA, sizeof(SocketAddress)) != 0)
    {
        perror("Bind failed\n");
        close(s);
    }
}

Socket::Socket(int port)
{
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
    }
    
    
    bzero(&originSA, sizeof(SocketAddress));
    makeLocalSA(&originSA, port);

    if (bind(s, (struct sockaddr*)&originSA, sizeof(SocketAddress)) != 0)
    {
        perror("Bind failed\n");
        close(s);
    }
    
}

status Socket::UDPsend(int           s,
                       Message *     message,
                       SocketAddress destination,
                        UPD_ENUM_COMMANDS cmd)
{
    int n, accumulative = 0;
    char packet[MAX_UDP_DATA_PACKET+1];
    int actual_packet_size;
    
    UDPPacketsHandler packetsHandler(message,cmd);
    
    while ( ! packetsHandler.is_transmission_reached_to_end() )
    {
        bzero(packet, MAX_UDP_DATA_PACKET);
        packetsHandler.get_next_packet(packet,actual_packet_size);
        
        if ((n = sendto(s, packet, actual_packet_size, 0, (struct sockaddr*) &destination,
                        sizeof(SocketAddress))) < 0)
        {
            perror("Send failed\n");
            //STATUS = BAD;
        }

        //if (n != message -> get_message_size())
        //    printf("sent partial data: %d\n", n);

        accumulative += n;
    }

    //if (accumulative == message->get_message_size() && accumulative != 0)
    //{
        printf("Packet(s) was sent successfully:  %d bytes\n", accumulative);
        //STATUS = OK;
    //}

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
        STATUS = BAD;
    }
    else
    {
        received_message[n] = '\0';
        //printf("Received Message: ( %s ), length = %d\n", received_message, n);
        STATUS = OK;
    }
    
    m->set_string(received_message,n);
    return STATUS;
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


