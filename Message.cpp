
#include "Message.h"

size_t Message::get_message_size()
{
    return size;
}

void Message::set_string(char *str, size_t size){
    //if ( data_array != nullptr )
    //    delete data_array;
    data_array = new char[size];
    memcpy(data_array,str,size);
    this->size = size;
}

Message::Message() : size(0), data_array(nullptr)
{
}

char *Message::get_data_without_header(){
    return data_array+HEADER_SIZE;
}

char *Message::get_complete_data(){
    return data_array;
}

void Message::debug_print_msg(){
    for(int i=HEADER_SIZE;i<size;i++){
        printf("%c",data_array[i]);
    }
    printf("\n");
}

Message::Message(const Message& msg){
    set_string(msg.data_array,msg.size);
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
    //buffer[len-1] = '\0';
}



bool Message::should_server_exit(){
    return data_array[0] == 'q';
}

Message::~Message(){
   // if ( data_array != nullptr)
   //     delete data_array;
}
