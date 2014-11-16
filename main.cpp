#include <iostream>
#include <stdio.h>
#include "Client.h"
#include "Server.h"
#include "Message.h"

#define NUMBER_THREADPOOL 4

using namespace std;

void usage(char ** argv)
{
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
            printf("Usage: s machine port message\n");
            exit(-1);
        }
        unsigned int port = atoi(argv[3]);
        Client  c;
        Message reply;
        Message sentMessage(argv[4]);

        c.echo_reply_Message(&sentMessage, &reply, argv[2], port);
    }
    else if (*argv[1] == 'r')
    {
        //Server  s(port);
        Server s;
        s.set_ThreadPool_size(NUMBER_THREADPOOL);
        
        for (;;){
             if ( s.wait_and_handle_clients() == -1 )
                 exit(-1);
        }
    }
    else
    {
        usage(argv);
    }

    return 0;
}


