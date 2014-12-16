#include "AUTHCommand.h"


Message AUTHCommand::construct_request_packet(std::string& user, std::string& pass){
    return Message(user+" "+pass);
}


void AUTHCommand::parse_request_packet(char *packet){
    UDPPacket udp_packet(packet);
    std::stringstream input(udp_packet.get_data());
    
    input >> username >> password;
}


void AUTHCommand::parse_response_packet(char *packet){
    UDPPacket udp_packet(packet);
    std::stringstream input(udp_packet.get_data());
    
    input >> session;
}

