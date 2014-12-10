#include "AuthHandler.h"

AuthHandler::AuthHandler() {}

void AuthHandler::initialize_handler(){
    std::string line;
    std::ifstream myfile(FILENAME);
    if (myfile.is_open())
    {
      while ( std::getline (myfile,line) )
      {
            std::istringstream iss (line);
            std::string username , passwd;
            iss >> username >> passwd;
            users_list.insert(std::pair<std::string,std::string>(username,passwd));
      }
      myfile.close();
    }

    else {
        printf("Unable to open authentication file\n");
        exit(1);
    }
    
}

bool AuthHandler::is_credentials_correct(std::string& user,std::string& pass){
     if ( users_list.count(user) > 0 ){
         return users_list[user] == pass;
     } else 
         return false;
}

std::string AuthHandler::create_new_session(std::string& user){
    char temp[SESSION_LENGTH+1];
    std::string session;
    do {
        Security::gen_random(temp, SESSION_LENGTH);
        temp[SESSION_LENGTH] = '\0';
        session = std::string(temp);
    } while ( sessions_storage.count(session) > 0 );
    
    sessions_storage.insert( std::pair<std::string,std::string>(session,user));
    return session;
}


bool AuthHandler::is_session_valid(std::string& session,std::string& username){
    if ( sessions_storage.count(session) > 0 ){
        username = sessions_storage[session];
        return true;
    } 
    return false;
}
