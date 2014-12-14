#include <iostream>
#include <stdio.h>
#include <string>
#include <arpa/inet.h>

#include "common.h"
#include "Client/EchoClient.h"
#include "Server/EchoServer.h"
#include "Payload/Message.h"

#define NUMBER_THREADPOOL 5

using namespace std;

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
            usage(argv);
        }
	else
		if( !isValidIpAddress(argv[2]) )
		{
			usage(argv);
		}

        unsigned int port = atoi(argv[3]);
        EchoClient  c;
   
        std::string message_to_echo(argv[4]);
        //printf("send: > ");
        //std::getline (std::cin,message_to_echo);
        //if ( message_to_echo == "quit") 
        //    break;

        Message reply;
        Message sentMessage(message_to_echo.c_str(),message_to_echo.length());
        c.start_client(&sentMessage, &reply, argv[2], port);
    }
    else if (*argv[1] == 'r')
    {
        EchoServer s(5000);
        s.set_ThreadPool_size(NUMBER_THREADPOOL);
        
        s.wait_and_handle_clients();
    }
    else
    {
        usage(argv);
    }

    return 0;
}

