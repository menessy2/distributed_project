/* 
 * File:   UDPPacketConstants.h
 * Author: root
 *
 * Created on November 25, 2014, 7:37 PM
 */

#ifndef UDPPACKETCONSTANTS_H
#define	UDPPACKETCONSTANTS_H


//      FOLLOWING NUMBERS ARE IN BYTES 

#define MAX_UDP_DATA_PACKET                 65507

#define SC_CHECKSUM_LENGTH                  4
#define SESSION_LENGTH                      10
#define TIMESTAMP_LENGTH                    4
#define SEQUENCE_NUMBER_LENGTH              4
#define REMAINING_PACKET_LEFT_LENGTH        4
#define COMMANDS_LENGTH                     1
#define WINDOW_LENGTH                       1
#define TOTAL_MSG_SIZE_LENGTH               4   // can support up to 4.29497 GB file
         


#define HEADER_SIZE     (SC_CHECKSUM_LENGTH+SESSION_LENGTH+TIMESTAMP_LENGTH+\
                        SEQUENCE_NUMBER_LENGTH+ REMAINING_PACKET_LEFT_LENGTH+\
                        COMMANDS_LENGTH+TOTAL_MSG_SIZE_LENGTH+WINDOW_LENGTH)     
                        
#define DATA_LENGTH     (MAX_UDP_DATA_PACKET-HEADER_SIZE)                

#define BYTE_SIZE   8


/////////////////////////////////////////////////////////////
// PACKET CONFIGURATION PARAMETERS

#define WINDOW_SIZE             10
#define KEEP_ALIVE_CONSTANT     2                           // in seconds
#define TIMEOUT_OF_REPLY        KEEP_ALIVE_CONSTANT + 10     // in seconds

#define MAGIC_NUMBER            5

#endif	/* UDPPACKETCONSTANTS_H */
