#ifndef UDPPACKET_H
#define	UDPPACKET_H

#include <sys/time.h>
#include <queue>
#include <string>
#include <stdint.h>
#include <ctype.h>

#include "../Payload/Message.h"
#include "../Misc/Security.h"
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
    size_t get_checksum() { return checksum; }
    UPD_ENUM_COMMANDS get_command() { return command; }
    bool is_checksum_correct() { return is_checksum_valid; }
    char *get_session() { return session; } 
    
    template <typename IntegerType>
    IntegerType bitsToInt(IntegerType& result, const unsigned char* bits, bool little_endian = false );
    
private:
    UPD_ENUM_COMMANDS command;
    unsigned int sequence_number;
    unsigned int remaining_packets;
    unsigned int timestamp;
    size_t checksum;
    std::string data;
    uint8_t window_size;
    unsigned int total_msg_filesize;
    char session[SESSION_LENGTH+1];
    bool is_checksum_valid;
};

bool operator<(const UDPPacket& udp1,const UDPPacket& udp2);
void clear_queue( std::queue<UDPPacket> &q );

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
    void get_next_packet(char *packet,unsigned int &size);
    void get_specific_packet(char *packet,unsigned int &size,unsigned int packetID);
    bool is_transmission_reached_to_end();
    
    // For receiving packets
    void add_UDPPacket(UDPPacket& packet);
    bool is_full_message_received();
    void overwrite_session(char *packet, char *session);
    void set_session(std::string& str);
    
    // more efficient than making a copy constructor
    void reset_Handler(unsigned int _size);
    
private:
    size_t cursor;        // represents the position from which the handler will start read from 
    unsigned int starting_sequence_number;
    Message *msg;
    unsigned int max_number_of_packets_to_receive;
    UPD_ENUM_COMMANDS command;
    unsigned int total_msg_size;
    std::string session;
    // For receiving packets
    std::priority_queue<UDPPacket> packets_vector;
    
    void construct_header(char *packet,UPD_ENUM_COMMANDS cmd=UPD_ENUM_COMMANDS::NA);
    void set_timestamp(char *buffer);
    void set_sequence_number(char *buffer);
    void set_remaining_packets(char *buffer);
    void set_command(char *buffer,UPD_ENUM_COMMANDS cmd);
    void set_window_size(char *buffer);
    void set_total_message_size(char *buffer);
    void set_dummy_session(char *buffer);
    
    void set_data_checksum(char *data_buffer,int data_size,char *packet);
    void overwrite_remaining_packets(char *packet,unsigned int packetID);
    
    unsigned int get_remaining_packets();
    unsigned int get_total_packets_number();
    
    void convert_int_to_bytes(unsigned int n, unsigned char* buffer);
    
    

};

#endif	/* UDPPACKET_H */

