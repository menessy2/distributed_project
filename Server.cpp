
#include "Server.h"

Server::Server(unsigned short port) : Socket(port)
{
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
    }
    bzero(&servaddr, sizeof(SocketAddress));
    makeReceiverSA(&servaddr);
    
    if (bind(sockfd,(struct sockaddr *)&servaddr,sizeof(SocketAddress)) != 0)
    {
        perror("Bind failed\n");
        close(sockfd);
    }
    
    socklen_t addr_size = sizeof(SocketAddress);
    getsockname(sockfd, (struct sockaddr*) &servaddr, &addr_size);
    printf("Welcome to SERVER MODE: \n");
    printf("\t. listening on port %d\n", ntohs(servaddr.sin_port));
    printf("\n---------------------------------\n");
    
}

status Server::receive_from_clients(Message * received_message)
{
    status get_request_status = GetRequest(received_message, sockfd, &servaddr);
    status reply_status       = SendReply(received_message, sockfd, servaddr );

    return reply_status;
}

status Server::GetRequest(Message *       callMessage,
                          int             s,
                          SocketAddress * clientSA)
{
    return Socket::UDPreceive(s, callMessage, clientSA);
}

status Server::SendReply(Message *replyMessage,int s, SocketAddress clientSA)
{
    return Socket::UDPsend(s, replyMessage, clientSA);
}

void Server::makeReceiverSA(SocketAddress * sa,
                            unsigned short  port)
{
    sa -> sin_family      = AF_INET;
    sa -> sin_port        = htons(port);
    sa -> sin_addr.s_addr = htonl(INADDR_ANY);
}


