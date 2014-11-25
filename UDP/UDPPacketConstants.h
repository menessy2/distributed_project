/* 
 * File:   UDPPacketConstants.h
 * Author: root
 *
 * Created on November 25, 2014, 7:37 PM
 */

#ifndef UDPPACKETCONSTANTS_H
#define	UDPPACKETCONSTANTS_H

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


#endif	/* UDPPACKETCONSTANTS_H */
