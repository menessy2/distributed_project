#include "ACKCommand.h"



Message ACKCommand::construct_packet(std::vector<unsigned int>* rhs){
    std::string msg="";
    for( auto& i : *rhs ){
        msg += std::to_string(i) + ";";
    }
    
    return Message(msg);
}


std::set<unsigned int> ACKCommand::parse_packet(char *packet){
    std::set<unsigned int> temp;
    UDPPacket udp_packet(packet);
    std::stringstream input(udp_packet.get_data());
    unsigned int element;
    while(std::getline(input, element, ';'))
    {
       temp.insert(element);
    }
    
    return temp;
}