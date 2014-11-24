
#include "Message.h"

size_t Message::get_message_size()
{
    return size;
}

void Message::set_string(char *str, size_t size){
    if ( data_array != nullptr )
        delete data_array;
    data_array = new char[size];
    memcpy(data_array,str,size);
    this->size = size;
}

Message::Message() : size(0), data_array(nullptr)
{
}

const char *Message::get_c_string(){
    return data_array;
}

Message::Message(char * m,size_t size) : data_array(nullptr)
{
    set_string(m,size);
}

void Message::split_string(size_t       start,
                            const size_t len,
                             char *buffer)
{
    memcpy(buffer,&data_array[start],len);
    buffer[len-1] = '\0';
}



bool Message::should_server_exit(){
    return data_array[0] == 'q';
}

Message::~Message(){
    if ( data_array != nullptr)
        delete data_array;
}
