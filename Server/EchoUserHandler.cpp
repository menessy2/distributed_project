/* 
 * File:   EchoUserHandler.cpp
 * Author: root
 * 
 * Created on December 3, 2014, 1:14 AM
 */

#include "EchoUserHandler.h"


EchoUserHandler::EchoUserHandler(const char* ip, uint16_t port, SocketAddress& sock_add, int& s) :
   UserHandler(ip,port,sock_add,s) 
 { }
        
        
void EchoUserHandler::message_notification(Message& msg){

    printf("Message was fully received from ( %s:%d ) : \n\tLength: %d bytes\n\tContent: %s\n\n",
                        inet_ntoa(destination.sin_addr),
                        ntohs(destination.sin_port),
                        msg.get_message_size(),
                        msg.get_string().c_str() );
    
}


void EchoUserHandler::server_Reaction_upon_success(Message& _msg){
    
    new std::thread(&Socket::UDPsend_ACK_support, &test_sock, sock_fd, _msg, destination,UPD_ENUM_COMMANDS::TRANSMIT_DATA);
        
    while ( true ) { 

        while ( messages_vector.empty() );

        Message expected_Ack = messages_vector.back();

        UDPPacket packet(expected_Ack.get_complete_data());


        refresh_keep_alive();
        switch( packet.get_command() ){
            case UPD_ENUM_COMMANDS::ACK:
                handle_Acks(expected_Ack.get_complete_data(),destination,sock_fd);
                break;
            case UPD_ENUM_COMMANDS::ACK_SUCCESS:
                test_sock.condition.notify_one();   // continue transmission with the new bulk of the window 
                break;
        }
        messages_vector.pop();
    }

}


void EchoUserHandler::action_when_user_reaches_timeout() {
    

}

void EchoUserHandler::client_Reaction_upon_success(Message& msg){

}
