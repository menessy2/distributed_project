#include "UDPPacket.h"

bool operator<(UDPPacket& udp1, UDPPacket& udp2){
    return udp1.sequence_number < udp2.sequence_number;
}


///////////////////////////////////////////////////////////////////////////////

UDPPacket::UDPPacket(char *packet){
    byte *timestamp_START = packet + SC_CHECKSUM_LENGTH;
    byte *sequence_number_START = timestamp_START + TIMESTAMP_LENGTH;
    byte *remaining_packets_START = sequence_number_START + SEQUENCE_NUMBER_LENGTH;
    byte *command_START = remaining_packets_START + COMMANDS_LENGTH;
    
    char timestamp_array[TIMESTAMP_LENGTH+1];
    char sequence_number_array[SEQUENCE_NUMBER_LENGTH+1];
    char remaining_packets_array[REMAINING_PACKET_LEFT_LENGTH+1];
    char command_array[COMMANDS_LENGTH+1];
    
    memcpy(checksum,packet,SC_CHECKSUM_LENGTH);
    memcpy(timestamp_array,timestamp_START,TIMESTAMP_LENGTH);
    memcpy(sequence_number_array,sequence_number_START,SEQUENCE_NUMBER_LENGTH);
    memcpy(remaining_packets_array,remaining_packets_START,REMAINING_PACKET_LEFT_LENGTH);
    memcpy(command_array,command_START,COMMANDS_LENGTH);
    
    command = static_cast<UPD_ENUM_COMMANDS>(command_array);
    sequence_number = atoi(sequence_number_array);
    remaining_packets = atoi(remaining_packets_array);
    timestamp = atol(timestamp_array);
}


///////////////////////////////////////////////////////////////////////////////

UDPPacketsHandler::UDPPacketsHandler(const Message& rhs, UPD_ENUM_COMMANDS cmd) :
data(rhs), command(cmd), cursor(0), max_number_of_packets_to_receive(0) {

    starting_sequence_number = rand() % (1 << SEQUENCE_NUMBER_LENGTH*BYTE_SIZE);
}

UDPPacketsHandler::UDPPacketsHandler(UPD_ENUM_COMMANDS cmd) : command(cmd), 
        max_number_of_packets_to_receive(0), cursor(0) {

    starting_sequence_number = rand() % (1 << SEQUENCE_NUMBER_LENGTH*BYTE_SIZE);
}

void UDPPacketsHandler::parse_UDPPacket(char *bytes_array){
    UDPPacket packet = UDPPacket(bytes_array);
    
    packets_vector.push(packet);
}


bool UDPPacketsHandler::is_full_message_received(){
    return packets_vector.size() == max_number_of_packets_to_receive;
}

void UDPPacketsHandler::get_next_packet(char *packet,int &size) {

    construct_header(packet);
    size = std::min(MAX_UDP_DATA_PACKET,(int)(this->data.length()- this->cursor) );
    packet = this->data.substr(cursor, size).c_str();
}

void UDPPacketsHandler::set_timestamp(byte *buffer) {
    unsigned long int longInt = time(NULL);

    buffer[0] = (byte) ((longInt & 0xFF000000) >> 24);
    buffer[1] = (byte) ((longInt & 0x00FF0000) >> 16);
    buffer[2] = (byte) ((longInt & 0x0000FF00) >> 8);
    buffer[3] = (byte) ((longInt & 0x000000FF));
}

void UDPPacketsHandler::set_sequence_number(byte *buffer) {
    starting_sequence_number += 1;
    sprintf(buffer, "%04x", starting_sequence_number);
}

void UDPPacketsHandler::set_remaining_packets(byte *buffer) {
    int remaining_packets = (data.length() - cursor) / DATA_LENGTH;
    sprintf (buffer, "%04x", remaining_packets);
}

void UDPPacketsHandler::set_command(byte *buffer){
    sprintf (buffer, "%02x", static_cast<int>(command) );
}

void UDPPacketsHandler::construct_header(byte *packet) {
    byte *timestamp_START = packet + SC_CHECKSUM_LENGTH;
    byte *sequence_number_START = timestamp_START + TIMESTAMP_LENGTH;
    byte *remaining_packets_START = sequence_number_START + SEQUENCE_NUMBER_LENGTH;
    byte *command_START = remaining_packets_START + COMMANDS_LENGTH;

    set_timestamp(timestamp_START);
    set_sequence_number(sequence_number_START);
    set_remaining_packets(remaining_packets_START);
}

