#include "Security.h"


std::string Security::calculate_hash(char *input){
    std::string result;
    
    std::string input2 = std::string(input)+std::string(SECURITY_PSK);
    /*
     * CryptoPP::SHA1 hash;
    CryptoPP::FileSource( ( input2.c_str() ),true,
        new CryptoPP::HashFilter(
            hash, new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(result), false)
        )
    );
     */
    return result;
}

bool Security::is_hash_matching_string(std::string &hash,std::string &str){
    return true;
}

