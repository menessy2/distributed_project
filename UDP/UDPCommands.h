#ifndef UDPCOMMANDS_H
#define	UDPCOMMANDS_H

enum class UPD_ENUM_COMMANDS  {
    IDLE,
    TRANSMIT_DATA,
    RETRANSMIT_REQUEST,
    RETRANSMIT_REPLY,
    PING,
    AUTH,
};

/*
class UDPCommands {
public:
    UDPCommands() = 0;
    virtual ~UDPCommands() = 0;
private:

};
*/
#endif	/* UDPCOMMANDS_H */

