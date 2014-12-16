#include "EchoServer.h"

EchoServer::EchoServer(int port) : Server(port) {
}


void EchoServer::dispatch_connection_to_UserHandler(Message *received_pkt,SocketAddress sck){
    std::string ip = std::string(inet_ntoa(sck.sin_addr));
    std::string port = std::to_string(ntohs(sck.sin_port));
    std::string result = ip+":"+port;
    if ( user_handlers.count(result) > 0 ){
        EchoUserHandler *handler = user_handlers[result];
        handler->notify_user_about_incomming_message(*received_pkt);
    } else {
        EchoUserHandler *user = new EchoUserHandler(ip.c_str(),ntohs(sck.sin_port),sck,sockfd);
        user_handlers.insert( std::pair<const char*,UserHandler*>(result.c_str(),
                user ) );
        
        //incomming_requests->enqueue( [=](){
        //    user->initialize_thread(*received_pkt,true);
        //});
        std::promise<bool> p;
        auto future = p.get_future();
        
        new std::thread(&EchoUserHandler::initialize_thread, user, *received_pkt,true,std::ref(p));
        
    }
}
