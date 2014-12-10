
#include <string>

#include "Message.h"

size_t Message::get_message_size()
{
    return size;
}

void Message::explicit_free(){
    delete data_array;
    size = 0;
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

Message::Message(std::string str){
    set_string(str.c_str(),str.length());
}

std::string Message::get_string(){
    std::string temp="";
    for(int i=0;i<size;i++){
        temp += data_array[i];
    }
    return temp;
}


std::string Message::get_data_string(){
    std::string temp="";
    for(int i=HEADER_SIZE;i<size;i++){
        temp += data_array[i];
    }
    return temp;
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

void Message::print_header(){
    for(int i=0;i<HEADER_SIZE;i++){
        printf("%02x",data_array[i]);
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
}


Message::~Message(){
   //if ( data_array != nullptr)
   //     delete data_array;
}
