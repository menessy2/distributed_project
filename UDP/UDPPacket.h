#ifndef UDPPACKET_H
#define	UDPPACKET_H

#include <sys/time.h>
#include <queue>
#include <string>
#include <stdint.h>
#include <ctype.h>

#include "../Payload/Message.h"
#include "Commands/UDPCommands.h"
#include "UDPPacketConstants.h"


// Used for parsing after receiving the packet
class UDPPacket{
public:
    UDPPacket(char *);
    friend bool operator<(const UDPPacket& udp1,const UDPPacket& udp2);
    unsigned int get_window_size() { return window_size; }
    unsigned int get_total_message_size() { return total_msg_filesize; }
    unsigned short get_remaining_packets()  { return remaining_packets; }
    std::string get_data() { return data; }
    unsigned int get_total_msg_size() { return total_msg_filesize; }
    UPD_ENUM_COMMANDS get_command() { return command; }
    
    template <typename IntegerType>
    IntegerType bitsToInt(IntegerType& result, const unsigned char* bits, bool little_endian = false );
    
private:
    UPD_ENUM_COMMANDS command;
    unsigned int sequence_number;
    unsigned int remaining_packets;
    unsigned int timestamp;
    char checksum[SC_CHECKSUM_LENGTH];
    std::string data;
    uint8_t window_size;
    unsigned int total_msg_filesize;
};

bool operator<(const UDPPacket& udp1,const UDPPacket& udp2);

class UDPPacketsHandler {
public:
    //UDPPacketsHandler(); //used only in Socket for Socket::UDPsend_ACK_support
    UDPPacketsHandler(Message *rhs,UPD_ENUM_COMMANDS command);
    UDPPacketsHandler(UPD_ENUM_COMMANDS command=UPD_ENUM_COMMANDS::IDLE);
    std::string get_whole_received_data();
    void set_total_msg_size(unsigned int size);
    void set_command(UPD_ENUM_COMMANDS cmd);
    void set_message(Message *rhs);
    
    // For sending packets
    void get_next_packet(char *packet,int &size);
    void get_specific_packet(char *packet,unsigned int &size,unsigned int packetID);
    bool is_transmission_reached_to_end();
    
    // For receiving packets
    void add_UDPPacket(UDPPacket& packet);
    bool is_full_message_received();
    
private:
    size_t cursor;        // represents the position from which the handler will start read from 
    unsigned int starting_sequence_number;
    Message *msg;
    unsigned int max_number_of_packets_to_receive;
    UPD_ENUM_COMMANDS command;
    unsigned int total_msg_size;
    
    // For receiving packets
    std::priority_queue<UDPPacket> packets_vector;
    
    void construct_header(char *packet,UPD_ENUM_COMMANDS cmd=UPD_ENUM_COMMANDS::NA);
    void set_timestamp(char *buffer);
    void set_sequence_number(char *buffer);
    void set_remaining_packets(char *buffer);
    void set_command(char *buffer,UPD_ENUM_COMMANDS cmd);
    void set_window_size(char *buffer);
    void set_total_message_size(char *buffer);
    
    unsigned int get_remaining_packets();
    unsigned int get_total_packets_number();
    
    void convert_int_to_bytes(unsigned int n, unsigned char* buffer);

};

#endif	/* UDPPACKET_H */

