#ifndef UDPPACKET_H
#define	UDPPACKET_H

#include <sys/time.h>
#include <queue>

#include "Message.h"
#include "UDPCommands.h"

//      FOLLOWING NUMBERS ARE IN BYTES 

#define MAX_UDP_DATA_PACKET                 65507

#define SC_CHECKSUM_LENGTH                  32
#define TIMESTAMP_LENGTH                    4
#define SEQUENCE_NUMBER_LENGTH              2
#define REMAINING_PACKET_LEFT_LENGTH        2
#define COMMANDS_LENGTH                     1

#define HEADER_SIZE     (SC_CHECKSUM_LENGTH+TIMESTAMP_LENGTH+SEQUENCE_NUMBER_LENGTH+\
                        REMAINING_PACKET_LEFT_LENGTH+COMMANDS_LENGTH)     
                        
#define DATA_LENGTH     (MAX_UDP_DATA_PACKET-HEADER_SIZE)                

#define BYTE_SIZE   8



// Used for parsing after receiving the packet
class UDPPacket{
public:
    UDPPacket(char *);
    friend bool operator<(UDPPacket& udp1, UDPPacket& udp2);
    
private:
    UPD_ENUM_COMMANDS command;
    unsigned short sequence_number;
    short remaining_packets;
    unsigned long int timestamp;
    char checksum[SC_CHECKSUM_LENGTH+1];
    std::string data;
};

bool operator<(UDPPacket& udp1, UDPPacket& udp2);

class UDPPacketsHandler {
public:
    UDPPacketsHandler(const Message& rhs,UPD_ENUM_COMMANDS command=UPD_ENUM_COMMANDS::TRANSMIT_DATA);
    UDPPacketsHandler(UPD_ENUM_COMMANDS command);
    virtual ~UDPPacketsHandler();
    
    // For sending packets
    void get_next_packet(char *packet);
    bool is_transmission_reached_to_end();
    
    // For receiving packets
    void parse_UDPPacket(char *);
    bool is_full_message_received();
    
    
private:
    size_t cursor;        // represents the position from which the handler will start read from 
    unsigned short starting_sequence_number;
    std::string data;
    unsigned int max_number_of_packets_to_receive;
    UPD_ENUM_COMMANDS command;
    
    // For receiving packets
    std::priority_queue<UDPPacket> packets_vector;
    
    void construct_header(byte *packet);
    void set_timestamp(byte *buffer);
    void set_sequence_number(byte *buffer);
    void set_remaining_packets(byte *buffer);
    void set_command(byte *buffer);

};

#endif	/* UDPPACKET_H */

