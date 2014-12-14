
#include "Server.h"

Server::Server(unsigned short port) 
{
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
    }
    bzero(&servaddr, sizeof(SocketAddress));
    makeReceiverSA(&servaddr,port);
    
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

void Server::set_ThreadPool_size(size_t threads_size){
    //incomming_requests = new ThreadPool(threads_size);
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

int Server::wait_and_handle_clients(){
    while( true ){
        Message received_msg;
        Socket::UDPreceive(sockfd, &received_msg, &servaddr);
        dispatch_connection_to_UserHandler(&received_msg,servaddr);
    }
    return 0;
}


Server::~Server(){
    //delete incomming_requests;
}
