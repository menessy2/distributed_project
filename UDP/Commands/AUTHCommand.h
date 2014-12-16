/* 
 * File:   AUTHCommand.h
 * Author: root
 *
 * Created on December 4, 2014, 1:00 PM
 */

#ifndef AUTHCOMMAND_H
#define	AUTHCOMMAND_H

#include <string>
#include <sstream>


#include "../UDPPacket.h"
#include "../../Payload/Message.h"

class AUTHCommand {
    
public:
    
    AUTHCommand() {}
    Message construct_request_packet(std::string& user, std::string& pass);
    Message construct_success_response_packet(std::string& session);
    void parse_request_packet(char *packet);
    void parse_response_packet(char *packet);
    std::string& get_session() { return session; }

    std::string username;
    std::string password;
    std::string session;
    
};

#endif	/* AUTHCOMMAND_H */

