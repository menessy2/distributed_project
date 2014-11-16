/* 
 * File:   UDPPacket.h
 * Author: root
 *
 * Created on November 16, 2014, 5:04 PM
 */

#ifndef UDPPACKET_H
#define	UDPPACKET_H


#define MAX_UDP_DATA_PACKET 6 //65507

class UDPPacket {
public:
    UDPPacket();
    UDPPacket(const UDPPacket& orig);
    virtual ~UDPPacket();
private:

};

#endif	/* UDPPACKET_H */

