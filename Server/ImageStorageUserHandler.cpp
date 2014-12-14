#include "ImageStorageUserHandler.h"

AuthHandler ImageStorageUserHandler::auth_handler;
std::string ImageStorageUserHandler::CLIENT_FILENAME;
std::map<std::string, UserHandler*> *ImageStorageUserHandler::user_handlers;

std::unordered_map<std::string,temporary_file_status> ImageStorageUserHandler::files_status;  

bool is_file_exist(std::string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

// do not forget to delete the returned pointer after usage
unsigned char* read_bytes_from_file(const char *name,size_t &len){
    std::streampos begin,end;
    std::ifstream file(name, std::ios::binary );
    begin = file.tellg();
    file.seekg( 0, std::ios::end );  
    end = file.tellg();
    len = end - begin;
    unsigned char *buffer = new unsigned char[len];
    file.seekg(0, std::ios::beg);   
    file.read(buffer, len);  
    file.close();  
    return buffer;  
}

bool is_directory_exist(std::string array){
    struct stat st;
    if(stat(array.c_str(),&st) == 0)
        return (st.st_mode & S_IFDIR != 0);
    return false;
}


ImageStorageUserHandler::ImageStorageUserHandler(const char* ip, uint16_t port, 
                                                 SocketAddress& sock_add, int& s) :
    UserHandler(ip,port,sock_add,s)  {}


void ImageStorageUserHandler::loop(){
    
    unsigned int accumulative_window,magic_counter=0;
    bool is_first_window_packet = true;
    
    while( true ){
        
        while ( messages_vector.empty() );
        
        Message msg = messages_vector.front();
        
        UDPPacket packet(messages_vector.front().get_complete_data());
        
        unsigned int packet_id = packet.get_remaining_packets();
        
        printf("received %d\n",packet_id);
        
        if ( is_packet_already_received(packet_id) /*|| ! packet.is_checksum_correct()*/ ){
            messages_vector.pop();
            //refresh_keep_alive();
            continue;
        }
        
        //printf("received2 %d\n",packet_id);
        
        if ( isServer )  {
            
            
            // check for session and authenticate
            if ( packet.get_command() != UPD_ENUM_COMMANDS::AUTH && 
                    packet.get_command() != UPD_ENUM_COMMANDS::ACK && 
                    packet.get_command() != UPD_ENUM_COMMANDS::ACK_SUCCESS ){
                std::string user,session=std::string(packet.get_session());
                if ( ! ImageStorageUserHandler::auth_handler.is_session_valid(session,user) ){
                    Message failure("failure");
                    Socket::UDPsend(sock_fd,&failure,destination,UPD_ENUM_COMMANDS::ACCESS_DENIED);
                    messages_vector.pop();
                    continue;
                }
            }
            
            switch( packet.get_command() ) {
                case UPD_ENUM_COMMANDS::AUTH:
                {
                    AUTHCommand auth_packet;
                    auth_packet.parse_request_packet(msg.get_complete_data());
                    if ( ImageStorageUserHandler::auth_handler.is_credentials_correct(auth_packet.username, auth_packet.password) ){
                        std::string session = ImageStorageUserHandler::auth_handler.create_new_session(auth_packet.username);
                        Message session_message(session);
                        Socket::UDPsend(sock_fd,&session_message,destination,UPD_ENUM_COMMANDS::AUTH_SUCCESS);
                        logged_user = auth_packet.username;
                    } else {
                        Message failure("failure");
                        Socket::UDPsend(sock_fd,&failure,destination,UPD_ENUM_COMMANDS::AUTH_FAILURE);
                    }
                    messages_vector.pop();
                    continue;
                }
                case UPD_ENUM_COMMANDS::CREATE_FILE:
                {
                    std::string original_filename = msg.get_data_string() ;    
                    filename = original_filename.substr( original_filename.find_last_of("\\/") + 1 );
                    if ( files_status.count(filename) > 0 ){
                        temporary_file_status temp_status = files_status[filename];
                        if ( temp_status.image_status == Image_Status::TRANSMITING && 
                                    temp_status.thread_id != std::this_thread::get_id() ){
                            Message failure("failure");
                            filename = "";
                            Socket::UDPsend(sock_fd,&failure,destination,UPD_ENUM_COMMANDS::CREATE_FILE_FAILURE);
                            messages_vector.pop();
                            continue;
                        } // else overwrite the file
                    } else {
                        temporary_file_status temp_status(std::this_thread::get_id(),Image_Status::TRANSMITING);
                        files_status.insert( std::pair<std::string,temporary_file_status>(filename,temp_status));
                    }
                    Message ok("ok");
                    Socket::UDPsend(sock_fd,&ok,destination,UPD_ENUM_COMMANDS::CREATE_FILE_SUCCESS);
                    messages_vector.pop();
                    packets_handler.reset_Handler(packet.get_total_msg_size());
                    continue;
                }
                case UPD_ENUM_COMMANDS::REQUEST_FILE:
                {
                    std::string original_filename = msg.get_data_string() ;
                    filename = original_filename.substr( original_filename.find_last_of("\\/") + 1 );
                    std::string _file = std::string(BASE_SERVER_STORAGE) + logged_user + "/" + filename ;
                    //printf("%s",_file.c_str());
                    if (  is_file_exist(_file) ){
                        size_t len;
                        unsigned char *image_bytes = read_bytes_from_file(_file.c_str(),len);
                        Message __msg(image_bytes,len);
                        new std::thread(&Socket::UDPsend_ACK_support, &test_sock, sock_fd, __msg, destination,UPD_ENUM_COMMANDS::TRANSMIT_DATA);
                    } else {
                        Message failure("failure");
                        Socket::UDPsend(sock_fd,&failure,destination,UPD_ENUM_COMMANDS::REQUEST_FILE_FAILURE);
                    }
                    messages_vector.pop();
                    continue;
                }
                case UPD_ENUM_COMMANDS::ACK:
                    handle_Acks(msg.get_complete_data(),destination,sock_fd);
                    messages_vector.pop();
                    continue;
                case UPD_ENUM_COMMANDS::ACK_SUCCESS:
                    test_sock.condition.notify_one();   // continue transmission with the new bulk of the window 
                    messages_vector.pop();
                    continue;
            }
        }
        
        if ( filename == "" && isServer ){
            messages_vector.pop();
            Message failure("failure");
            Socket::UDPsend(sock_fd,&failure,destination,UPD_ENUM_COMMANDS::ACCESS_DENIED);
            continue;
        }
        
        //      TRASMITDATA & RETRANSMIT ARE ONLY ALLOWED HERE
        
        // packets_handler.reset_Handler(packet.get_total_msg_size());
        
        if ( is_first_window_packet ){
            accumulative_window = packet.get_window_size();
            packets_handler.set_total_msg_size(packet.get_total_msg_size());
            is_first_window_packet = false;
        }
        
        packets_handler.add_UDPPacket( packet );

        packets_received_within_a_window.push_back(packet.get_remaining_packets());
        
        if ( ++magic_counter % MAGIC_NUMBER == 0)
            std::this_thread::sleep_for (std::chrono::milliseconds(500));
        
        if ( ( ++window_counter % accumulative_window == 0) ) {
            // send acknowledgment
            ACKCommand ack;
            Message msg = ack.construct_packet(&packets_received_within_a_window);
            Socket::UDPsend(sock_fd,&msg,destination,UPD_ENUM_COMMANDS::ACK_SUCCESS);
            packets_received_within_a_window.clear();
            printf("* Acknowledgment of %d packets was sent *\n",window_counter);
            window_counter = 0;
            accumulative_window = packet.get_window_size();
        }
        // Conditions based on the UDP Packet
        
        messages_vector.pop();
        
        if ( packets_handler.is_full_message_received() )
            break;
        
        refresh_keep_alive();
    }
    
    if ( isServer ){
        user_handlers->erase(std::string(connected_ip)+":"+std::to_string(port));
    }
    
    Message _msg(packets_handler.get_whole_received_data());
    
    message_notification(_msg);
    
    clear_queue(messages_vector);
    
    //keep_alive_periodic_checker.destroy(tid);
    
    if ( isServer )
        server_Reaction_upon_success(_msg);
    else
        client_Reaction_upon_success(_msg);
    
    
}



void ImageStorageUserHandler::message_notification(Message& msg){
    printf("---------------------------------------------\n");
    if ( isServer )
        printf("Image was successfully received: %s @ %s\n",logged_user.c_str(),filename.c_str());
    else
        printf("Image was successfully received: %s\n",CLIENT_FILENAME.c_str());
    
}



void ImageStorageUserHandler::server_Reaction_upon_success(Message& msg){
    
    temporary_file_status temp_status(std::this_thread::get_id(),Image_Status::DONE);
    
    
    int status;

    if ( ! is_directory_exist(std::string(BASE_DIR) + logged_user) ){
        std::string dir = std::string(BASE_DIR) + logged_user;
        status = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    
    std::string whole_filepath = std::string(BASE_DIR) + logged_user + "/" + filename;
    std::ofstream outfile (whole_filepath.c_str(),std::ofstream::binary);
    outfile.write(msg.get_complete_data(),msg.get_message_size());
    
    msg.explicit_free();
    outfile.close();
    
    files_status[filename] = temp_status;
    
}

void ImageStorageUserHandler::client_Reaction_upon_success(Message& msg){
    
    std::string whole_filepath = std::string(BASE_CLIENT_STORAGE) + "/" + CLIENT_FILENAME;
    std::ofstream outfile (whole_filepath.c_str(),std::ofstream::binary);
    outfile.write(msg.get_complete_data(),msg.get_message_size());
    
    msg.explicit_free();
    outfile.close();
}

void ImageStorageUserHandler::action_when_user_reaches_timeout() {
    temporary_file_status temp_status(std::this_thread::get_id(),Image_Status::DONE);
    files_status[filename] = temp_status;
    
    if ( isServer ){
        user_handlers->erase(std::string(connected_ip)+":"+std::to_string(port));
    }
}
