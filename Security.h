#ifndef SECURITY_H
#define	SECURITY_H

#include <string>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>

#define SECURITY_PSK "this is a password"

static class Security {
public:
    
    static std::string calculate_hash(char *);
    static bool is_hash_matching_string(std::string &hash,std::string &str);

};

#endif	/* SECURITY_H */

