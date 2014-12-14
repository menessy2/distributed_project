#include "Security.h"


size_t Security::calculate_hash(unsigned char *input,unsigned int array_length){
    size_t result;
    /*
    auto d = sizeof(SECURITY_PSK);
    unsigned char *array = new unsigned char[array_length+sizeof(SECURITY_PSK)];
    array[array_length+sizeof(SECURITY_PSK)-1] = '\0';
   
    memcpy(array,input,array_length);
    memcpy(array+array_length,SECURITY_PSK,sizeof(SECURITY_PSK));
    */
    std::string checksum = std::string(input) + std::string(SECURITY_PSK);
    
    //std::hash<unsigned char *> str_hash;
    std::hash<std::string> str_hash;
    result = str_hash(checksum);
    /*
     * CryptoPP::SHA1 hash;
    CryptoPP::FileSource( ( input2.c_str() ),true,
        new CryptoPP::HashFilter(
            hash, new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(result), false)
        )
    );
     */
    //delete array;
    return result;
}

bool Security::is_hash_matching_string(size_t hash, char *str, unsigned int length){
    return hash == calculate_hash(str,length);
}


void Security::gen_random(char *s, const int len) {
    srand (time(NULL));
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

