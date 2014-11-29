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
    std::string element;
    while(std::getline(input, element, ';'))
    {
        if ( ! std::isdigit(element[0]) )
            break;
        unsigned int tmp_num = std::stoi (element,nullptr,0);
        temp.insert(tmp_num);
    }
    
    return temp;
}