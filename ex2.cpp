#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <arpa/inet.h>
//#include <QApplication>

//#include "Server/GUI/GUI/mainwindow.h"
#include "common.h"
#include "Client/ImageStorageClient.h"
#include "Server/ImageStorageServer.h"
#include "Payload/Image.h"

#define NUMBER_THREADPOOL 1

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
        
        
        std::string username = std::string(argv[4]);
        std::string pass = std::string(argv[5]);
        
        
        
        std::vector<std::string> list_of_images_to_send;
        list_of_images_to_send.push_back(argv[6]);
        //list_of_images_to_send.push_back("Image.mp3");
        
        //c.request_image("image1.png");
        for ( auto& image_path : list_of_images_to_send ){
            ImageStorageClient  c;
            c.initialize_client(argv[2], port);
            c.authenticate(username,pass);
            Image temp_image(image_path);
            c.send_image(&temp_image);
        }
        /*
        printf("asdasd\n");
        ImageStorageClient  c;
        c.initialize_client(argv[2], port);
        c.authenticate(username,pass);
        c.request_image("image1.png");
        */
        
    }
    else if (*argv[1] == 'r')
    {
        ImageStorageServer s(5000);
        s.set_ThreadPool_size(NUMBER_THREADPOOL);
        
        s.wait_and_handle_clients();
    }
    else
    {
        usage(argv);
    }

    return 0;
}

