CC=g++

ADDIT = -std=c++11 -Wl,--no-as-needed -pthread -ggdb -fpermissive -w
CFLAGS=-c $(ADDIT)

all: ex1 ex2

ex1: 	ex1.o 			\
	Misc/Security.o Misc/timer.o	\
	Payload/Message.o	\
	UDP/Socket.o UDP/UDPPacket.o UDP/Commands/ACKCommand.o \
	Server/Server.o Server/ThreadPool.o Server/UserHandler.o  Server/EchoServer.o\
	Client/Client.o Client/EchoClient.o Server/EchoUserHandler.o
	$(CC) $(ADDIT) ex1.o Message.o ACKCommand.o \
		Socket.o UDPPacket.o EchoClient.o EchoServer.o EchoUserHandler.o \
		Server.o ThreadPool.o UserHandler.o  \
		Client.o timer.o Security.o -o ex_1


ex2: 	ex2.o	\
	Misc/Security.o Misc/timer.o	\
	Payload/Image.o Payload/Message.o	\
	UDP/Socket.o UDP/UDPPacket.o UDP/Commands/ACKCommand.o UDP/Commands/AUTHCommand.o \
	Server/Server.o Server/ThreadPool.o Server/UserHandler.o  Server/ImageStorageServer.o\
	Client/Client.o Client/ImageStorageClient.o Server/ImageStorageUserHandler.o Server/Auth/AuthHandler.o
	$(CC) $(ADDIT) ex2.o Message.o Image.o ACKCommand.o AuthHandler.o \
		Socket.o UDPPacket.o ImageStorageClient.o ImageStorageServer.o ImageStorageUserHandler.o \
		Server.o ThreadPool.o UserHandler.o  AUTHCommand.o \
		Client.o timer.o Security.o -o executable


ex1.o: ex1.cpp
	$(CC) $(CFLAGS) ex1.cpp

ex2.o: ex2.cpp
	$(CC) $(CFLAGS) ex2.cpp
	#qmake -makefile -o ./Server/GUI/GUI/Makefile ./Server/GUI/GUI/
	#make -C ./Server/GUI/GUI/
	#qmake -makefile -o ./Client/GUI/GUI/Makefile ./Client/GUI/GUI/
	#make -C ./Client/GUI/GUI/
	

Misc/Security.o: Misc/Security.cpp Misc/Security.h
	$(CC) $(CFLAGS) Misc/Security.cpp

Misc/timer.o: Misc/timer.cpp  Misc/timer.h
	$(CC) $(CFLAGS) Misc/timer.cpp

Payload/Message.o: Payload/Message.cpp Payload/Message.h
	$(CC) $(CFLAGS) Payload/Message.cpp

Payload/Image.o: Payload/Image.cpp Payload/Image.h
	$(CC) $(CFLAGS) Payload/Image.cpp

UDP/Socket.o: UDP/Socket.cpp UDP/Socket.h
	$(CC) $(CFLAGS) UDP/Socket.cpp

UDP/Commands/ACKCommand.o : UDP/Commands/ACKCommand.cpp UDP/Commands/ACKCommand.h
	$(CC) $(CFLAGS) UDP/Commands/ACKCommand.cpp

UDP/Commands/AUTHCommand.o : UDP/Commands/AUTHCommand.cpp UDP/Commands/AUTHCommand.h
	$(CC) $(CFLAGS) UDP/Commands/AUTHCommand.cpp

UDP/UDPPacket.o: UDP/UDPPacket.h UDP/UDPPacket.cpp
	$(CC) $(CFLAGS) UDP/UDPPacket.cpp

UDP/UDPPacket.o: UDP/UDPPacket.cpp UDP/UDPPacket.h
	$(CC) $(CFLAGS) UDP/UDPPacket.cpp

Server/Server.o: Server/Server.h Server/Server.cpp
	$(CC) $(CFLAGS) Server/Server.cpp

Server/EchoServer.o: Server/EchoServer.h Server/EchoServer.cpp
	$(CC) $(CFLAGS) Server/EchoServer.cpp

Server/ImageStorageServer.o: Server/ImageStorageServer.h Server/ImageStorageServer.cpp
	$(CC) $(CFLAGS) Server/ImageStorageServer.cpp
	 

Server/Auth/AuthHandler.o: Server/Auth/AuthHandler.h Server/Auth/AuthHandler.cpp
	$(CC) $(CFLAGS) Server/Auth/AuthHandler.cpp


Server/ThreadPool.o: Server/ThreadPool.h Server/ThreadPool.cpp
	$(CC) $(CFLAGS) Server/ThreadPool.cpp



Server/UserHandler.o: Server/UserHandler.h Server/UserHandler.cpp
	$(CC) $(CFLAGS) Server/UserHandler.cpp

Server/ImageStorageUserHandler.o: Server/ImageStorageUserHandler.h Server/ImageStorageUserHandler.cpp
	$(CC) $(CFLAGS) Server/ImageStorageUserHandler.cpp

Server/EchoUserHandler.o: Server/EchoUserHandler.h Server/EchoUserHandler.cpp
	$(CC) $(CFLAGS) Server/EchoUserHandler.cpp



Client/EchoClient.o: Client/EchoClient.h Client/EchoClient.cpp
	$(CC) $(CFLAGS) Client/EchoClient.cpp

Client/ImageStorageClient.o: Client/ImageStorageClient.h Client/ImageStorageClient.cpp
	$(CC) $(CFLAGS) Client/ImageStorageClient.cpp

Client/Client.o: Client/Client.h Client/Client.cpp
	$(CC) $(CFLAGS) Client/Client.cpp


clean:
	rm -rf *o ex
	#make clean -C ./Server/GUI/GUI/
	#make clean -C ./Client/GUI/GUI/
