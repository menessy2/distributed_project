
#include "Message.h"

size_t Message::get_message_size()
{
    return data.length();
}

void Message::set_string(char *str){
    data = std::string(str);
}

Message::Message()
{
}

const char *Message::get_c_string(){
    return data.c_str();
}

Message::Message(char * m)
{
    data = std::string(m);
}

const char * Message::split_string(size_t       start,
                                   const size_t len)
{
    return data.substr(start, len).c_str();
}


bool Message::should_server_exit(const Message &rhs){
    return this->data == "q";
}
