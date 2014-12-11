#ifndef IMAGESTORAGECLIENT_H
#define	IMAGESTORAGECLIENT_H

#include <fstream> 
#include <iostream>

#include "Client.h"
#include "../Server/ImageStorageUserHandler.h"
#include "../Payload/Image.h"
#include "../UDP/Commands/AUTHCommand.h"
#include "../Misc/FileLocker.h"


class ImageStorageClient : public Client {
    
public:
    
    ImageStorageClient();
    void initialize_client(char *recepient, unsigned short port);
    
    void listener();
    void authenticate(std::string& user, std::string& pass);
    void send_image(Image *image_obj);
    void request_image(std::string image_name);
    virtual status DoOperation (Message *message, Message *reply, int s, SocketAddress serverSA);
    
private:
    
    
    char *_recepient;
    unsigned short _port;
    
    std::thread *client_listener;
    
    std::string username;
    std::string pass;
    std::string session;
    
};

#endif	/* IMAGESTORAGECLIENT_H */

