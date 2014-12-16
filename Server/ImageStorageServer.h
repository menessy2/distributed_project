#ifndef IMAGESTORAGE_H
#define IMAGESTORAGE_H

#include "Server.h"
#include "ImageStorageUserHandler.h"
#include "../Payload/Image.h"
#include "../UDP/UDPPacket.h"
#include "../UDP/Commands/AUTHCommand.h"
#include "../UDP/Commands/UDPCommands.h"

#include <unordered_map>
#include <string>

class ImageStorageServer : public Server {
    
    public:
        
        ImageStorageServer(int);
        virtual void dispatch_connection_to_UserHandler(Message *received_pkt,SocketAddress sck);
  
   	//void clear_garabage(){  sent_packets.clear();received_packets.clear(); }
    
};


#endif
