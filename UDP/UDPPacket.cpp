#include "UDPPacket.h"

bool operator<(const UDPPacket& udp1, const UDPPacket& udp2){
    return udp1.remaining_packets < udp2.remaining_packets;
}

void clear_queue( std::priority_queue<UDPPacket> &q )
{
   std::priority_queue<UDPPacket> empty;
   std::swap( q, empty );
}

///////////////////////////////////////////////////////////////////////////////


UDPPacket::UDPPacket(char *packet){
    unsigned const char *checksum_START = const_cast<char *>(packet);
    unsigned const char *session_START = checksum_START + SC_CHECKSUM_LENGTH;
    unsigned const char *timestamp_START = session_START + SESSION_LENGTH;
    unsigned const char *sequence_number_START = timestamp_START + TIMESTAMP_LENGTH;
    unsigned const char *remaining_packets_START = sequence_number_START + SEQUENCE_NUMBER_LENGTH;
    unsigned const char *command_START = remaining_packets_START + REMAINING_PACKET_LEFT_LENGTH;
    unsigned const char *window_size_START = command_START + COMMANDS_LENGTH;
    unsigned const char *total_message_size_START = window_size_START + WINDOW_LENGTH;
    unsigned const char *data_ptr = total_message_size_START + TOTAL_MSG_SIZE_LENGTH;
    
    
    unsigned char checksum_array[SC_CHECKSUM_LENGTH];
    unsigned char timestamp_array[TIMESTAMP_LENGTH];
    unsigned char sequence_number_array[SEQUENCE_NUMBER_LENGTH];
    unsigned char remaining_packets_array[REMAINING_PACKET_LEFT_LENGTH];
    unsigned char command_array[COMMANDS_LENGTH];
    unsigned char window_array[WINDOW_LENGTH];
    unsigned char total_message_size_array[TOTAL_MSG_SIZE_LENGTH];
    
    
    memcpy(checksum_array,packet,SC_CHECKSUM_LENGTH);
    memcpy(session,session_START,SESSION_LENGTH);
    memcpy(timestamp_array,timestamp_START,TIMESTAMP_LENGTH);
    memcpy(sequence_number_array,sequence_number_START,SEQUENCE_NUMBER_LENGTH);
    memcpy(remaining_packets_array,remaining_packets_START,REMAINING_PACKET_LEFT_LENGTH);
    memcpy(command_array,command_START,COMMANDS_LENGTH);
    memcpy(window_array,window_size_START,WINDOW_LENGTH);
    memcpy(total_message_size_array,total_message_size_START,TOTAL_MSG_SIZE_LENGTH);
    
    uint8_t command_number;
    command_number = bitsToInt<uint8_t>(command_number, command_array);
    command = static_cast<UPD_ENUM_COMMANDS>(command_number);
    sequence_number = bitsToInt<unsigned int>(sequence_number, sequence_number_array);
    remaining_packets = bitsToInt<unsigned int>(remaining_packets, remaining_packets_array);
    timestamp = bitsToInt<unsigned int>(timestamp, timestamp_array);
    window_size = bitsToInt<uint8_t>(window_size, window_array);
    total_msg_filesize = bitsToInt<unsigned int>(total_msg_filesize, total_message_size_array);
    checksum = bitsToInt<unsigned int>( (unsigned int &)checksum, checksum_array);
    
    session[SESSION_LENGTH] = '\0';
    
    data = "";
    
    if ( remaining_packets >= 1){
        for ( int i=0; i < DATA_LENGTH ;i++){
            data += data_ptr[i];
        }
    }
    else{
        for ( int i=0; i < total_msg_filesize % DATA_LENGTH ;i++){
            data += data_ptr[i];
        }
    }
    
    is_checksum_valid = Security::is_hash_matching_string(checksum, data_ptr, DATA_LENGTH);

}

template <typename IntegerType>
IntegerType UDPPacket::bitsToInt(IntegerType& result, const unsigned char* bits, bool little_endian= false  ){
    result = 0;
    if (little_endian)
        for (int n = sizeof( result ); n >= 0; n--)
            result = (result << 8) +bits[ n ];
    else
        for (unsigned n = 0; n < sizeof( result ); n++)
            result = (result << 8) +bits[ n ];
    return result;
}


///////////////////////////////////////////////////////////////////////////////

UDPPacketsHandler::UDPPacketsHandler(Message *rhs, UPD_ENUM_COMMANDS cmd) :
msg(rhs), command(cmd), cursor(0), max_number_of_packets_to_receive(0) {
    srand(time(0));
    starting_sequence_number = static_cast<unsigned int>(rand() % (1 << (SEQUENCE_NUMBER_LENGTH-1)*BYTE_SIZE)-1);
    total_msg_size = rhs->get_message_size();
}

void UDPPacketsHandler::reset_Handler(unsigned int _size){
    set_total_msg_size(_size);
    clear_queue(packets_vector);
}

UDPPacketsHandler::UDPPacketsHandler(UPD_ENUM_COMMANDS cmd) : command(cmd), 
        max_number_of_packets_to_receive(0), cursor(0) {
    srand(time(0));
    starting_sequence_number = rand() % (1 << (SEQUENCE_NUMBER_LENGTH-1)*BYTE_SIZE)-1;
}

std::string UDPPacketsHandler::get_whole_received_data(){
    std::string temp="";
    std::priority_queue<UDPPacket> temp_pq = packets_vector;
    while(!temp_pq.empty()) {
        temp += temp_pq.top().get_data();
        temp_pq.pop();
    }
    return temp;
}

bool UDPPacketsHandler::is_transmission_reached_to_end(){
    return ( cursor + 1 ) >= msg->get_message_size();
}

void UDPPacketsHandler::add_UDPPacket(UDPPacket& packet){
    packets_vector.push(packet);
}


bool UDPPacketsHandler::is_full_message_received(){
    return packets_vector.size() == max_number_of_packets_to_receive;
}


void UDPPacketsHandler::set_command(UPD_ENUM_COMMANDS cmd){
    command = cmd;
}


void UDPPacketsHandler::set_message(Message *rhs){
    msg = rhs;
}


void UDPPacketsHandler::get_next_packet(char *packet,unsigned int &size) {
    construct_header(packet);
    size = std::min<unsigned int>(DATA_LENGTH,(unsigned int)(msg->get_message_size()- this->cursor) );
    char buffer[DATA_LENGTH];
    bzero(buffer,DATA_LENGTH);
    msg->split_string(cursor, size, buffer);
    memcpy(packet+HEADER_SIZE, buffer ,size);
    cursor += size;
    size += HEADER_SIZE;
    
    set_data_checksum(buffer,DATA_LENGTH,packet);
    if ( session != "" )
        overwrite_session(packet,session.c_str());
}

void UDPPacketsHandler::get_specific_packet(char *packet,unsigned int &size,unsigned int packetID){
    construct_header(packet,UPD_ENUM_COMMANDS::RETRANSMIT);
    overwrite_remaining_packets(packet,packetID);
    unsigned int start = ( get_total_packets_number() - packetID - 1) * DATA_LENGTH;
    unsigned int temp = (unsigned int)msg->get_message_size()-start;
    size = std::min<unsigned int>(DATA_LENGTH,temp);
    char buffer[DATA_LENGTH];
    bzero(buffer,DATA_LENGTH);
    msg->split_string(start, size, buffer);
    memcpy(packet+HEADER_SIZE, buffer ,size);
    size += HEADER_SIZE;
    
    set_data_checksum(buffer,DATA_LENGTH,packet);
    if ( session != "" )
        overwrite_session(packet,session.c_str());
}

void UDPPacketsHandler::overwrite_remaining_packets(char *packet,unsigned int packetID){
    unsigned char *remaining_packets_START = packet + \
                    SC_CHECKSUM_LENGTH+SESSION_LENGTH+TIMESTAMP_LENGTH+SEQUENCE_NUMBER_LENGTH;
    convert_int_to_bytes(packetID, remaining_packets_START);
}

void UDPPacketsHandler::overwrite_session(char *packet, char *session){
    unsigned char *session_START = packet + SC_CHECKSUM_LENGTH;
    memcpy(session_START,session,SESSION_LENGTH);
}

void UDPPacketsHandler::set_total_msg_size(unsigned int _size)  {
    total_msg_size = _size;
    max_number_of_packets_to_receive =  total_msg_size / DATA_LENGTH;
    if ( total_msg_size % DATA_LENGTH != 0 )
        max_number_of_packets_to_receive += 1;
}

void UDPPacketsHandler::set_timestamp(char *buffer) {
    unsigned long int longInt = time(NULL);
    convert_int_to_bytes(longInt, buffer);
}

void UDPPacketsHandler::set_sequence_number(char *buffer) {
    starting_sequence_number += 1;
    convert_int_to_bytes(starting_sequence_number, buffer);
}

void UDPPacketsHandler::set_remaining_packets(char *buffer) {
    unsigned int remaining_packets = get_remaining_packets();
    convert_int_to_bytes(remaining_packets, buffer);
}

void UDPPacketsHandler::set_command(char *buffer,UPD_ENUM_COMMANDS cmd){
    if ( cmd == UPD_ENUM_COMMANDS::NA )
        buffer[0] = (unsigned char) (static_cast<uint8_t>(command) );
    else
        buffer[0] = (unsigned char) (static_cast<uint8_t>(cmd) );
}

void UDPPacketsHandler::set_window_size(char *buffer){
    unsigned int remaining_packets = get_remaining_packets();
    if ( remaining_packets >= WINDOW_SIZE )
        buffer[0] = (unsigned char) static_cast<uint8_t>(WINDOW_SIZE);
    else if ( ( get_total_packets_number() % WINDOW_SIZE ) == 0 )
        buffer[0] = (unsigned char) static_cast<uint8_t>(WINDOW_SIZE);
    else
        buffer[0] = (unsigned char) static_cast<uint8_t>((get_total_packets_number() % WINDOW_SIZE ));
}

void UDPPacketsHandler::set_total_message_size(char *buffer){
    convert_int_to_bytes(total_msg_size, buffer);
}

void UDPPacketsHandler::set_dummy_session(char *buffer){
    bzero(buffer,SESSION_LENGTH);
}

void UDPPacketsHandler::set_data_checksum(char *data_buffer,int data_size,char *packet){
    size_t checksum = Security::calculate_hash(data_buffer,data_size);
    convert_int_to_bytes(checksum, packet);
}

void UDPPacketsHandler::construct_header(char *packet,UPD_ENUM_COMMANDS cmd) {
    unsigned char *checksum_START = const_cast<char *>(packet);
    unsigned char *session_START = checksum_START + SC_CHECKSUM_LENGTH;
    unsigned char *timestamp_START = session_START + SESSION_LENGTH;
    unsigned char *sequence_number_START = timestamp_START + TIMESTAMP_LENGTH;
    unsigned char *remaining_packets_START = sequence_number_START + SEQUENCE_NUMBER_LENGTH;
    unsigned char *command_START = remaining_packets_START + REMAINING_PACKET_LEFT_LENGTH;
    unsigned char *window_size_START = command_START + COMMANDS_LENGTH;
    unsigned char *total_message_size_START = window_size_START + WINDOW_LENGTH;
    
    set_dummy_session(session_START);
    set_timestamp(timestamp_START);
    set_sequence_number(sequence_number_START);
    set_remaining_packets(remaining_packets_START);
    set_command(command_START,cmd);
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


void UDPPacketsHandler::convert_int_to_bytes(unsigned int n, unsigned char* buffer){
    buffer[0] = (unsigned char) ((n & 0xFF000000) >> 24);
    buffer[1] = (unsigned char) ((n & 0x00FF0000) >> 16);
    buffer[2] = (unsigned char) ((n & 0x0000FF00) >> 8);
    buffer[3] = (unsigned char) ((n & 0x000000FF));
}



void UDPPacketsHandler::set_session(std::string& str){
    session = str;
}




