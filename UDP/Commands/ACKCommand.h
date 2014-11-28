#ifndef ACKCOMMAND_H
#define ACKCOMMAND_H

#include <vector>
#include <string>
#include <sstream>
#include <set>

#include "../../Payload/Message.h"
#include "../UDPPacket.h"
#include "UDPCommands.h"

class ACKCommand /*: public UDPCommands*/ {
    public:
        ACKCommand() {}
        Message construct_packet(std::vector<unsigned int>* rhs);
        std::set<unsigned int> parse_packet(char *packet);

};




#endif