#include "EchoClient.h"

EchoClient::EchoClient(){ }


status EchoClient::DoOperation(Message *     message,
                           Message *     reply,
                           int           s,
                           SocketAddress serverSA)
{
    new std::thread(&Socket::UDPsend_ACK_support, this, s, *message, serverSA,UPD_ENUM_COMMANDS::TRANSMIT_DATA);
    
    EchoUserHandler *user_handler = new EchoUserHandler(inet_ntoa(serverSA.sin_addr),ntohs(serverSA.sin_port),serverSA,s);
    bool is_first_time_to_send_data=true;
    
    refresh_keep_alive();
    run_keep_alive_check();
    
    while ( true ) { 
        Message expected_Ack;
        Socket::UDPreceive(s, &expected_Ack, &serverSA);
    
        UDPPacket packet(expected_Ack.get_complete_data());
        
        refresh_keep_alive();
        switch( packet.get_command() ){
            case UPD_ENUM_COMMANDS::ACK:
                handle_Acks(expected_Ack.get_complete_data(),serverSA,s);
                break;
            case UPD_ENUM_COMMANDS::ACK_SUCCESS:
                condition.notify_one();   // continue transmission with the new bulk of the window 
                break;
            case UPD_ENUM_COMMANDS::TRANSMIT_DATA:
            case UPD_ENUM_COMMANDS::RETRANSMIT:
                if ( is_first_time_to_send_data ){
                    new std::thread(&EchoUserHandler::initialize_thread, user_handler, expected_Ack,false,std::ref(p));
                    is_first_time_to_send_data = false;
                } else
                    user_handler->notify_user_about_incomming_message(expected_Ack);
                break;
        }
        expected_Ack.explicit_free();
    }
        
    close(s);
    return status::OK;
}

