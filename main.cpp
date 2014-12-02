#include <iostream>
#include <stdio.h>
#include "Client/Client.h"
#include "Server/Server.h"
#include "Payload/Message.h"
#include <arpa/inet.h>

#define NUMBER_THREADPOOL 2

using namespace std;

bool isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}


void usage(char ** argv)
{
	//
    printf("Usage:s(end) ...or r(eceive) ??\n");
    printf("ex: %s s machine port message\n", argv[0]);
    printf("ex: %s r\n", argv[0]);
    exit(-1);
}

int main(int argc,char ** argv)
{
    if (argc <= 1)
    {
        usage(argv);
    }

    if (*argv[1] == 's')
    {
        if (argc <= 2)
        {
            printf("Usage: s machineID portNumber message\n");
            exit(-1);
        }
	else
		if( !isValidIpAddress(argv[2]) )
		{
			printf("Enter a Valid IP address \n");
 			printf("Usage: s machineID portNumber message\n");
			exit(-1);
		}



        unsigned int port = atoi(argv[3]);
        Client  c;
        Message reply;
        Message sentMessage(argv[4],strlen(argv[4]));

        c.echo_reply_Message(&sentMessage, &reply, argv[2], port);
    }
    else if (*argv[1] == 'r')
    {
        //Server  s(port);
        Server s;
        s.set_ThreadPool_size(NUMBER_THREADPOOL);
        
        s.wait_and_handle_clients();
    }
    else
    {
        usage(argv);
    }

    return 0;
}

