#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/types.h>
#include <cstring>
#include <stdio.h>

#include "../UDP/UDPPacketConstants.h"

class Message {

public:
    Message();
    Message(const Message& msg);
    Message(std::string str);
    ~Message();
    Message(char *m,size_t size);
    size_t get_message_size();
    void set_string(char *str, size_t size);
    void split_string(size_t start, const size_t len, char *buffer);
    char *get_complete_data();
    char *get_data_without_header();
    void debug_print_msg();
    void print_header();
    std::string get_string();
    std::string get_data_string();
    
    void explicit_free();

protected:
    size_t size;   
    char *data_array;
    //std::unique_ptr<char> data_array;
};

enum status { 
	OK, 		// operation successful
	BAD, 		// unrecoverable error
	WRONGLENGTH 	// bad message length supplied
};



typedef struct sockaddr_in SocketAddress;

#endif
