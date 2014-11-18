
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
    status request_status = Socket::UDPsend(s, message, serverSA);
    status reply_status   = Socket::UDPreceive(s, reply, &originSA);

    close(s);

    return reply_status;
}


