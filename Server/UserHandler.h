#ifndef USERHANDLER_H
#define	USERHANDLER_H

#include <thread>

#include "../UDP/UDPPacket.h"

class UserHandler {
public:
    UserHandler();
    UserHandler(const UserHandler& orig);
    virtual ~UserHandler();
private:
    int keep_alive;
    UDPPacketsHandler packets_handler; 
};

#endif	/* USERHANDLER_H */

