#include "UDPPacket.h"

UDPPacketsHandler::UDPPacketsHandler(const Message& rhs, UPD_ENUM_COMMANDS cmd) :
data(rhs), command(cmd), cursor(0) {

    starting_sequence_number = rand() % (1 << SEQUENCE_NUMBER_LENGTH*BYTE_SIZE);
}

UDPPacketsHandler::UDPPacketsHandler(UPD_ENUM_COMMANDS cmd) : command(cmd), cursor(0) {

    starting_sequence_number = rand() % (1 << SEQUENCE_NUMBER_LENGTH*BYTE_SIZE);
}

void get_next_packet(char *packet) {

    construct_header(packet);

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

