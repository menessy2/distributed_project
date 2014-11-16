#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <sys/types.h>

class Message {

public:
        Message();
        ~Message();
	Message(char *m);
	size_t get_message_size();
        void set_string(char *str);
        const char *split_string(size_t start, size_t end);
        const char *get_c_string();
        bool should_server_exit();

private:

	std::string data; 
};

enum status { 
	OK, 		// operation successful
	BAD, 		// unrecoverable error
	WRONGLENGTH 	// bad message length supplied
};



typedef struct sockaddr_in SocketAddress;

#endif
