#include "ImageStorageClient.h"

ImageStorageClient::ImageStorageClient() {}


void ImageStorageClient::initialize_client(char *recepient, unsigned short port){
    Socket::makeDestSA(&destinationSA, recepient, port);
    _recepient = recepient;
    _port = port;
    
    client_listener = new std::thread(&ImageStorageClient::listener, this);
}


void ImageStorageClient::authenticate(std::string& user, std::string& pass){
    
    AUTHCommand auth_cmd;
    Message msg = auth_cmd.construct_request_packet(user, pass);
    Socket::UDPsend_ACK_support(s, msg, destinationSA,UPD_ENUM_COMMANDS::AUTH);
}

void ImageStorageClient::send_image(Image *image_obj){
    
    Message reply;

    std::string filepath = image_obj->get_filepath();
    
    if ( ! is_file_exist(filepath) ){
        printf("Sorry, file does not exist!\n");
    } else {
        std::string filename = filepath.substr( filepath.find_last_of("\\/") + 1 );
        //size_t len;
        //unsigned char *image = read_bytes_from_file(filepath.c_str(),len);

        Message message(filename);
        Socket::UDPsend_ACK_support2(s, message, destinationSA,UPD_ENUM_COMMANDS::CREATE_FILE,session);

        
        Socket::UDPsend_ACK_support_with_fd(s, filename.c_str(), destinationSA,UPD_ENUM_COMMANDS::TRANSMIT_DATA,session);
        
        //delete image;
    }
}

void ImageStorageClient::delete_dir(){
    
    Message msg("delete");
    UDPsend_ACK_support2(s, msg, destinationSA,UPD_ENUM_COMMANDS::DELETE_DIR_REQUEST,session);
    
}


void ImageStorageClient::request_image(std::string image_name){
    //Socket::makeLocalSA(&destinationSA);
    //close(s);
    
    Message msg(image_name);
    ImageStorageUserHandler::CLIENT_FILENAME = image_name;
    Socket::UDPsend_ACK_support2(s, msg, destinationSA,UPD_ENUM_COMMANDS::REQUEST_FILE,session);
}


// FREAKING ASYNC CALLBACK
void ImageStorageClient::listener(){        
    
    ImageStorageUserHandler *user_handler = new ImageStorageUserHandler(inet_ntoa(destinationSA.sin_addr),ntohs(destinationSA.sin_port),destinationSA,s);
    bool is_first_time_to_send_data=true;
    
    refresh_keep_alive();
    run_keep_alive_check();
    
    while ( true ) { 
        Message expected_Ack;
        
        Socket::UDPreceive(s, &expected_Ack, &destinationSA);
    
        UDPPacket packet(expected_Ack.get_complete_data());
        
        refresh_keep_alive();
        switch( packet.get_command() ){
            case UPD_ENUM_COMMANDS::ACK:
                handle_Acks(expected_Ack.get_complete_data(),destinationSA,s);
                break;
            case UPD_ENUM_COMMANDS::ACK_SUCCESS:
                condition.notify_one();   // continue transmission with the new bulk of the window 
                break;
            case UPD_ENUM_COMMANDS::TRANSMIT_DATA:
            case UPD_ENUM_COMMANDS::RETRANSMIT:
                if ( is_first_time_to_send_data ){
                    new std::thread(&ImageStorageUserHandler::initialize_thread, user_handler, expected_Ack,false,std::ref(p));
                    is_first_time_to_send_data = false;
                } else
                    user_handler->notify_user_about_incomming_message(expected_Ack);
                break;
                
            case UPD_ENUM_COMMANDS::AUTH_SUCCESS:
            {
                AUTHCommand auth_cmd;
                auth_cmd.parse_response_packet(expected_Ack.get_complete_data());
                session = auth_cmd.get_session();
                printf("Authentication was successful\n");
                printf("Session: %s",session.c_str());
                condition.notify_one();
                break;
            }
            case UPD_ENUM_COMMANDS::AUTH_FAILURE:
                printf("Authentication Failed\n");
                break;
            case UPD_ENUM_COMMANDS::ACCESS_DENIED:
                printf("Access Denied\n");
                break;
            case UPD_ENUM_COMMANDS::RECEIVE_SUCCESS:
                goto exit;
            case UPD_ENUM_COMMANDS::CREATE_FILE_SUCCESS:
                printf("File has been accepted, and currently being transfered\n");
                condition.notify_one();
                break;
            case UPD_ENUM_COMMANDS::CREATE_FILE_FAILURE:
                printf("File has been refused, *probably same filename is being sent concurrently\n");
                break;
            case UPD_ENUM_COMMANDS::REQUEST_FILE_FAILURE:
                printf("Requested file does not exist\n");
                break;
            case UPD_ENUM_COMMANDS::DELETE_DIR_RESPONSE:
                printf("Delete request was sent to the server\n");
                break;
                
                
        }
        expected_Ack.explicit_free();
    }

exit:    
    
    close(s);
    
}

status ImageStorageClient::DoOperation(Message *     message,
                           Message *     reply,
                           int           s,
                           SocketAddress serverSA)
{
    //new std::thread(&Socket::UDPsend_ACK_support, this, s, *message, serverSA,UPD_ENUM_COMMANDS::TRANSMIT_DATA);
    
    Socket::UDPsend_ACK_support2(s, *message, serverSA,UPD_ENUM_COMMANDS::TRANSMIT_DATA,session);
    return status::OK;
}







