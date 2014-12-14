
#ifndef AUTHHANDLER_H
#define	AUTHHANDLER_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../../Misc/Security.h"
#include "../../UDP/UDPPacketConstants.h"


#define FILENAME "Server/Auth/users_config.list"




struct Account {
    
    std::string username;
    std::string password;
    
    Account(std::string& user,std::string& pass) : username(user) , password(pass) {}
    Account(const Account& rhs) : username(rhs.username) , password(rhs.password) { }
};


class AuthHandler {

public:

    AuthHandler();
    void initialize_handler();
    bool is_credentials_correct(std::string& user,std::string& pass);
    std::string create_new_session(std::string& user);
    bool is_session_valid(std::string& session,std::string& username);

private:

    // user - pass
    std::unordered_map<std::string,std::string> users_list;
    
    // session - User
    std::unordered_map<std::string,std::string> sessions_storage;
    
};

#endif	/* AUTHHANDLER_H */

