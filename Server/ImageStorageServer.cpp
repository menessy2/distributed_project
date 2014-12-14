
#include "ImageStorageServer.h"


ImageStorageServer::ImageStorageServer(int port) : Server(port) {
    ImageStorageUserHandler::auth_handler.initialize_handler();
    ImageStorageUserHandler::user_handlers = &user_handlers;
}


void ImageStorageServer::dispatch_connection_to_UserHandler(Message *received_pkt,SocketAddress sck){
    
    std::string ip = std::string(inet_ntoa(sck.sin_addr));
    std::string port = std::to_string(ntohs(sck.sin_port));
    std::string result = ip+":"+port;
    if ( user_handlers.count(result) > 0 ){
        ImageStorageUserHandler *handler = user_handlers[result];
        handler->notify_user_about_incomming_message(*received_pkt);
    } else {
        ImageStorageUserHandler *user = new ImageStorageUserHandler(ip.c_str(),ntohs(sck.sin_port),sck,sockfd);
        user_handlers.insert( std::pair<const char*,UserHandler*>(result.c_str(),
                user ) );
        
        std::promise<bool> p;
        auto future = p.get_future();
        
        new std::thread(&ImageStorageUserHandler::initialize_thread, user, *received_pkt,true,std::ref(p));
        
    }
}