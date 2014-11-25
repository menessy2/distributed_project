#ifndef UDPPACKET_H
#define	UDPPACKET_H

#include <sys/time.h>
#include <queue>
#include <string>

#include "../Message.h"
#include "UDPCommands.h"
#include "UDPPacketConstants.h"
//      FOLLOWING NUMBERS ARE IN BYTES 





// Used for parsing after receiving the packet
class UDPPacket{
public:
    UDPPacket(char *);
    friend bool operator<(const UDPPacket& udp1,const UDPPacket& udp2);
private:
    UPD_ENUM_COMMANDS command;
    unsigned short sequence_number;
    short remaining_packets;
    unsigned long int timestamp;
    char checksum[SC_CHECKSUM_LENGTH+1];
    std::string data;
};

bool operator<(const UDPPacket& udp1,const UDPPacket& udp2);

class UDPPacketsHandler {
public:
    UDPPacketsHandler(Message *rhs,UPD_ENUM_COMMANDS command=UPD_ENUM_COMMANDS::TRANSMIT_DATA);
    UDPPacketsHandler(UPD_ENUM_COMMANDS command=UPD_ENUM_COMMANDS::IDLE);
    const char *get_data();
    
    // For sending packets
    void get_next_packet(char *packet,int &size);
    bool is_transmission_reached_to_end();
    
    // For receiving packets
    void parse_UDPPacket(char *);
    bool is_full_message_received();
    
    
private:
    size_t cursor;        // represents the position from which the handler will start read from 
    unsigned short starting_sequence_number;
    Message *msg;
    unsigned int max_number_of_packets_to_receive;
    UPD_ENUM_COMMANDS command;
    
    // For receiving packets
    std::priority_queue<UDPPacket> packets_vector;
    
    void construct_header(char *packet);
    void set_timestamp(char *buffer);
    void set_sequence_number(char *buffer);
    void set_remaining_packets(char *buffer);
    void set_command(char *buffer);

};

#endif	/* UDPPACKET_H */

