#ifndef UDPCOMMANDS_H
#define	UDPCOMMANDS_H

enum class UPD_ENUM_COMMANDS  {
    TRANSMIT_DATA,
    RETRANSMIT_REQUEST,
    RETRANSMIT_REPLY,
    PING
};


class UDPCommands {
public:
    UDPCommands() = 0;
    virtual ~UDPCommands() = 0;
private:

};

#endif	/* UDPCOMMANDS_H */

