#ifndef UDPCOMMANDS_H
#define	UDPCOMMANDS_H

#include <vector>

enum class UPD_ENUM_COMMANDS  {
    NA,                     // UNINITIALIZED
    IDLE,                   // FOR RECEPIENTS
    TRANSMIT_DATA,
    RETRANSMIT,
    ACK,
    ACK_SUCCESS,
    PING,
};

/*
class UDPCommands {
public:
    UDPCommands() = 0;
    virtual ~UDPCommands() = 0;
//private:

};
 */

#endif	/* UDPCOMMANDS_H */

