/* 
 * File:   ImageStorageUserHandler.h
 * Author: root
 *
 * Created on December 3, 2014, 1:14 AM
 */

#ifndef IMAGESTORAGEUSERHANDLER_H
#define	IMAGESTORAGEUSERHANDLER_H

#define BASE_DIR "Server/__STORAGE__/"

#include <thread>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>

#include "UserHandler.h"
#include "Auth/AuthHandler.h"
#include "../UDP/Commands/AUTHCommand.h"
#include "../Misc/FileLocker.h"

#define BASE_CLIENT_STORAGE "Client/__STORAGE__/"
#define BASE_SERVER_STORAGE "Server/__STORAGE__/"

#define LOGGING "Server/__LOGGING__/logging.txt"

enum class Image_Status  {
    TRANSMITING,
    DONE
};

unsigned char* read_bytes_from_file(const char *name,size_t &len);

bool is_file_exist(std::string& name);

class temporary_file_status {
    
public:
    
    std::thread::id thread_id;
    Image_Status image_status;
    
    temporary_file_status() {}
    
    temporary_file_status(std::thread::id id, Image_Status image){
        thread_id = id;
        image_status = image;
    }
    
    temporary_file_status(const temporary_file_status& status){
        thread_id = status.thread_id;
        image_status = status.image_status;
    }
};

bool is_directory_exist(std::string array);

class ImageStorageUserHandler : public UserHandler {
    
public:
    
    ImageStorageUserHandler(const char*, uint16_t, SocketAddress&, int&);
    virtual void loop();
    virtual void message_notification(Message& msg);
    virtual void server_Reaction_upon_success(Message& msg);
    virtual void action_when_user_reaches_timeout();
    virtual void client_Reaction_upon_success(Message& msg);

    int remove_directory(const char *path);
    
    static AuthHandler auth_handler;
    static std::string CLIENT_FILENAME;
    static  std::map<std::string, UserHandler*> *user_handlers;
    
private:
        
    std::string logged_user;
    std::string filename;
    // hashmap ID : is username-filename
    static std::unordered_map<std::string,temporary_file_status> files_status;    
    
};

#endif	/* IMAGESTORAGEUSERHANDLER_H */

