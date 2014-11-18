#ifndef UDPPACKET_H
#define	UDPPACKET_H

#include <sys/time.h>

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

class UDPPacketsHandler {
public:
    UDPPacketsHandler(const Message& rhs,UPD_ENUM_COMMANDS command=UPD_ENUM_COMMANDS::TRANSMIT_DATA);
    UDPPacketsHandler(UPD_ENUM_COMMANDS command);
    void get_next_packet(char *packet);
    bool is_transmission_reached_to_end();
    virtual ~UDPPacketsHandler();
private:
    size_t cursor;        // represents the position from which the handler will start read from 
    unsigned short starting_sequence_number;
    std::string data;
    UPD_ENUM_COMMANDS command;
    
    void construct_header(byte *packet);
    void set_timestamp(byte *buffer);
    void set_sequence_number(byte *buffer);
    void set_remaining_packets(byte *buffer);
    void set_command(byte *buffer);

};

#endif	/* UDPPACKET_H */

