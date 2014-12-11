
#include "Client.h"

Client::Client()
{
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket creation failed");
    }
    
    bzero(&destinationSA, sizeof(SocketAddress));
    makeLocalSA(&destinationSA);
    
    if (bind(s, (struct sockaddr*)&destinationSA, sizeof(SocketAddress)) != 0) {
        perror("Bind failed\n");
        close(s);
    }
    printf("Welcome to CLIENT MODE:\n");
}

status Client::start_client(Message *      message,
                                  Message *      reply,
                                  char *         recepient,
                                  unsigned short port)
{
    Socket::makeDestSA(&destinationSA, recepient, port);

    return DoOperation(message, reply, s, destinationSA);
}

void Client::run_keep_alive_check(){
    tid = keep_alive_periodic_checker.create(0, 1000,
             [&]() {
                 std::chrono::seconds sec(TIMEOUT_OF_REPLY);
                 auto client_death_difference = std::chrono::duration_cast<std::chrono::seconds>(sec);
                 auto result =  std::chrono::system_clock::now() - keep_alive ;
                 if ( result > client_death_difference ){
                     // call destructor
                     printf("Server is disconnected.\n");
                     keep_alive_periodic_checker.destroy(tid);
                     exit(0);
                 } 
                 
                 auto status = future_thread.wait_for(std::chrono::milliseconds(0));

                if (status == std::future_status::ready) {
                    printf("Transmission done\n");
                    exit(0);
                }
                
                 
             });
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
        printf("%d, ", packet_id );
    }
    printf("\n");
}


void Client::refresh_keep_alive(){
    keep_alive = std::chrono::system_clock::now();
}

