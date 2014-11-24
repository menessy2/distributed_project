#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/types.h>
#include <cstring>

class Message {

public:
    Message();
    ~Message();
    Message(char *m,size_t size);
    size_t get_message_size();
    void set_string(char *str, size_t size);
    void split_string(size_t start, const size_t len, char *buffer);
    const char *get_c_string();
    bool should_server_exit();

private:
    size_t size;    
    char *data_array;
	//std::string data; 
    
};

enum status { 
	OK, 		// operation successful
	BAD, 		// unrecoverable error
	WRONGLENGTH 	// bad message length supplied
};



typedef struct sockaddr_in SocketAddress;

#endif
