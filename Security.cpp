#include "Security.h"


static std::string Security::calculate_hash(char *input){
    std::string result;
    CryptoPP::SHA1 hash;
    CryptoPP::FileSource( ( input+SECURITY_PSK ),true,
        new CryptoPP::HashFilter(
            hash, new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(result), false)
        )
    );
    return result;
}

