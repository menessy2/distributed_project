#include "ACKCommand.h"



Message ACKCommand::construct_packet(std::vector<unsigned int>& rhs){
    std::string msg="";
    for( auto& i : rhs ){
        msg += std::to_string(i) + ";";
    }
    
    return Message(msg);
}