#ifndef SECURITY_H
#define	SECURITY_H

#include <cstring>
#include <string>
#include <stdlib.h> 
#include <functional>
//#include <cryptopp/sha.h>
//#include <cryptopp/hex.h>
//#include <cryptopp/files.h>

#define SECURITY_PSK "this is a password"

class Security {
public:
    
    static size_t calculate_hash(unsigned char *input,unsigned int array_length);
    static bool is_hash_matching_string(size_t hash, char *str, unsigned int length);
    static void gen_random(char *s, const int len);

};

#endif	/* SECURITY_H */

