#ifndef ACKCOMMAND_H
#define ACKCOMMAND_H

#include <vector>
#include <string>

#include "../../Payload/Message.h"
#include "UDPCommands.h"

class ACKCommand /*: public UDPCommands*/ {
    public:
        ACKCommand() {}
        Message construct_packet(std::vector<unsigned int>& rhs);

};




#endif