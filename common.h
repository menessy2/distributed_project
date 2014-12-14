/* 
 * File:   common.h
 * Author: root
 *
 * Created on December 3, 2014, 11:57 AM
 */

#ifndef COMMON_H
#define	COMMON_H

#include <arpa/inet.h>

bool isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}


void usage(char ** argv)
{
    printf("Usage:s(end) ...or r(eceive) ??\n");
    printf("ex: %s s machine port message\n", argv[0]);
    printf("ex: %s r\n", argv[0]);
    exit(-1);
}


#endif	/* COMMON_H */

