
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

void Server::set_ThreadPool_size(size_t threads_size){
    outgoing_responses  = new ThreadPool(threads_size);
    incomming_requests = new ThreadPool(threads_size);
}

void Server::dispatch_connection_to_UserHandler(const char *received_msg,SocketAddress sck){
    std::string ip = std::string(inet_ntoa(sck.sin_addr));
    std::string port = std::to_string(ntohs(sck.sin_port));
    std::string result = ip+":"+port;
    if ( user_handlers.count(result) > 0 ){
        UserHandler *handler = &user_handlers[result];
        handler->notify_user_about_incomming_message(received_msg);
    } else {
        UserHandler user = UserHandler(ip.c_str(),ntohs(sck.sin_port));
        user_handlers.insert( std::pair<const char*,UserHandler>(result.c_str(),
                user ) );
        user.initialize_thread(received_msg);
    }
    
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


Server::~Server(){
    delete outgoing_responses;
    delete incomming_requests;
}


int Server::wait_and_handle_clients(){
    Message received_msg;
    Socket::UDPreceive(sockfd, &received_msg, &servaddr);
    //dispatch_connection_to_UserHandler(received_msg.get_c_string(),servaddr);
    
    incomming_requests->enqueue( [=](){
            std::string s = received_msg.copy_message();
            char *msg = const_cast<char *>(s.c_str());
            dispatch_connection_to_UserHandler(msg,servaddr);
            SendReply(&const_cast<Message&>(received_msg), sockfd, servaddr );
        } 
    );
    
    /*
    if ( received_msg.should_server_exit() ){
        printf("Server is exiting ...\n");
        return -1;
    } 
    
    
    outgoing_responses->enqueue( [=](){
            //get_request_status = GetRequest(received_message, sockfd, &servaddr);
            SendReply(&const_cast<Message&>(received_msg), sockfd, servaddr );
        } 
    );
    */    
    //handle_client(&received_msg);
    return 0;
}

