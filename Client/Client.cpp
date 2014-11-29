
#include "Client.h"

Client::Client()
{
    printf("Welcome to CLIENT MODE:\n");
}

status Client::echo_reply_Message(Message *      message,
                                  Message *      reply,
                                  char *         recepient,
                                  unsigned short port)
{
    Socket::makeDestSA(&destinationSA, recepient, port);

    return DoOperation(message, reply, s, destinationSA);
}

status Client::DoOperation(Message *     message,
                           Message *     reply,
                           int           s,
                           SocketAddress serverSA)
{
    //ServerHandler handler;
    
    status request_status = Socket::UDPsend_ACK_support(s, message, serverSA,UPD_ENUM_COMMANDS::TRANSMIT_DATA);
    //status reply_status   = Socket::UDPreceive(s, reply, &originSA);
    std::string ip = std::string(inet_ntoa(serverSA.sin_addr));
    UserHandler user = UserHandler(ip.c_str(),ntohs(serverSA.sin_port),serverSA,s);
    status reply_status   = Socket::UDPreceive(s, reply, &originSA);
    user.initialize_thread(*reply,false);
    
    printf("Whole message received from client: %s",user.get_whole_data().c_str());
    
    std::string data = user.get_whole_data();
    reply = &Message(data);
    
    close(s);

    return reply_status;
}


