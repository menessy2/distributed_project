#include "UDPPacket.h"

bool operator<(const UDPPacket& udp1, const UDPPacket& udp2){
    return udp1.sequence_number < udp2.sequence_number;
}


///////////////////////////////////////////////////////////////////////////////
UDPPacket::UDPPacket(char *packet){
    char *timestamp_START = const_cast<char *>(packet) + SC_CHECKSUM_LENGTH;
    char *sequence_number_START = timestamp_START + TIMESTAMP_LENGTH;
    char *remaining_packets_START = sequence_number_START + SEQUENCE_NUMBER_LENGTH;
    char *command_START = remaining_packets_START + COMMANDS_LENGTH;
    char *window_size_START = command_START + COMMANDS_LENGTH;
    char *total_message_size_START = window_size_START + WINDOW_LENGTH;
    
    char timestamp_array[TIMESTAMP_LENGTH+1];
    char sequence_number_array[SEQUENCE_NUMBER_LENGTH+1];
    char remaining_packets_array[REMAINING_PACKET_LEFT_LENGTH+1];
    char command_array[COMMANDS_LENGTH+1];
    char window_array[WINDOW_LENGTH+1];
    char total_message_size_array[TOTAL_MSG_SIZE_LENGTH+1];
    
    memcpy(checksum,packet,SC_CHECKSUM_LENGTH);
    memcpy(timestamp_array,timestamp_START,TIMESTAMP_LENGTH);
    memcpy(sequence_number_array,sequence_number_START,SEQUENCE_NUMBER_LENGTH);
    memcpy(remaining_packets_array,remaining_packets_START,REMAINING_PACKET_LEFT_LENGTH);
    memcpy(command_array,command_START,COMMANDS_LENGTH);
    memcpy(window_array,window_size_START,WINDOW_LENGTH);
    memcpy(total_message_size_array,total_message_size_START,TOTAL_MSG_SIZE_LENGTH);
    
    command = static_cast<UPD_ENUM_COMMANDS>(atoi(command_array));
    sequence_number = atoi(sequence_number_array);
    remaining_packets = atoi(remaining_packets_array);
    timestamp = atol(timestamp_array);
    window_size = atoi(window_array);
    total_msg_filesize = atol(total_message_size_array);
}

///////////////////////////////////////////////////////////////////////////////

UDPPacketsHandler::UDPPacketsHandler(Message *rhs, UPD_ENUM_COMMANDS cmd) :
msg(rhs), command(cmd), cursor(0), max_number_of_packets_to_receive(0) {

    starting_sequence_number = rand() % (1 << SEQUENCE_NUMBER_LENGTH*BYTE_SIZE);
    total_msg_size = rhs->get_message_size();
}

UDPPacketsHandler::UDPPacketsHandler(UPD_ENUM_COMMANDS cmd) : command(cmd), 
        max_number_of_packets_to_receive(0), cursor(0) {

    starting_sequence_number = rand() % (1 << SEQUENCE_NUMBER_LENGTH*BYTE_SIZE);
}

const char *UDPPacketsHandler::get_data(){
    return msg->get_data_without_header();
}

bool UDPPacketsHandler::is_transmission_reached_to_end(){
    return ( cursor + 1 ) >= msg->get_message_size();
}

UDPPacket UDPPacketsHandler::parse_UDPPacket(char *bytes_array){
    UDPPacket packet = UDPPacket(bytes_array);
    
    packets_vector.push(packet);
    return packet;
}


bool UDPPacketsHandler::is_full_message_received(){
    return packets_vector.size() == max_number_of_packets_to_receive;
}

void UDPPacketsHandler::get_next_packet(char *packet,int &size) {

    construct_header(packet);
    size = std::min(MAX_UDP_DATA_PACKET,(int)(msg->get_message_size()- this->cursor) );
    char *buffer = new char[size];
    msg->split_string(cursor, size, buffer);
    memcpy(packet+HEADER_SIZE, buffer ,size);
    cursor += size;
    size += HEADER_SIZE;
    delete buffer;
}

void UDPPacketsHandler::set_timestamp(char *buffer) {
    unsigned long int longInt = time(NULL);

    buffer[0] = (char) ((longInt & 0xFF000000) >> 24);
    buffer[1] = (char) ((longInt & 0x00FF0000) >> 16);
    buffer[2] = (char) ((longInt & 0x0000FF00) >> 8);
    buffer[3] = (char) ((longInt & 0x000000FF));
}

void UDPPacketsHandler::set_sequence_number(char *buffer) {
    starting_sequence_number += 1;
    sprintf(buffer, "%04x", starting_sequence_number);
}

void UDPPacketsHandler::set_remaining_packets(char *buffer) {
    unsigned int remaining_packets = get_remaining_packets();
    sprintf (buffer, "%04x", remaining_packets);
}

void UDPPacketsHandler::set_command(char *buffer){
    sprintf (buffer, "%02x", static_cast<int>(command) );
}

void UDPPacketsHandler::set_window_size(char *buffer){
    unsigned int remaining_packets = get_remaining_packets();
    if ( remaining_packets >= WINDOW_SIZE)
        sprintf (buffer, "%02x", WINDOW_SIZE );
    else
        sprintf (buffer, "%02x", get_total_packets_number() % WINDOW_SIZE );
}

void UDPPacketsHandler::set_total_message_size(char *buffer){
    sprintf (buffer, "%04x", total_msg_size);
}

void UDPPacketsHandler::construct_header(char *packet) {
    char *timestamp_START = packet + SC_CHECKSUM_LENGTH;
    char *sequence_number_START = timestamp_START + TIMESTAMP_LENGTH;
    char *remaining_packets_START = sequence_number_START + SEQUENCE_NUMBER_LENGTH;
    char *command_START = remaining_packets_START + REMAINING_PACKET_LEFT_LENGTH;
    char *window_size_START = command_START + COMMANDS_LENGTH;
    char *total_message_size_START = window_size_START + WINDOW_LENGTH;
    
    set_timestamp(timestamp_START);
    set_sequence_number(sequence_number_START);
    set_remaining_packets(remaining_packets_START);
    set_command(command_START);
    set_window_size(window_size_START);
    set_total_message_size(total_message_size_START);
}



unsigned int UDPPacketsHandler::get_remaining_packets(){
    return (msg->get_message_size() - cursor) / DATA_LENGTH;
}

unsigned int UDPPacketsHandler::get_total_packets_number(){
    unsigned int total_packets = total_msg_size / DATA_LENGTH;
    if ( total_msg_size % DATA_LENGTH != 0 )
        total_packets += 1;
    return total_packets;
}