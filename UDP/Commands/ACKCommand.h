#ifndef ACKCOMMAND_H
#define ACKCOMMAND_H

#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <set>

#include "../../Payload/Message.h"
#include "../UDPPacket.h"
#include "UDPCommands.h"

class ACKCommand {
    public:
        ACKCommand() {}
        Message construct_packet(std::vector<unsigned int>* rhs);
        std::set<unsigned int> parse_packet(char *packet);

};




#endif