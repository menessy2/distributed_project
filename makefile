CC=g++

ADDIT = -std=c++11 -Wl,--no-as-needed -pthread -ggdb -fpermissive
CFLAGS=-c $(ADDIT)

all: hello

hello: main.o Message.o Security.o \
	UDP/Socket.o UDP/UDPCommands.o UDP/UDPPacket.o \
	Server/Server.o Server/ThreadPool.o Server/UserHandler.o  \
	Client/Client.o
	$(CC) $(ADDIT) main.o Message.o Security.o \
		Socket.o UDPCommands.o UDPPacket.o \
		Server.o ThreadPool.o UserHandler.o  \
		Client.o -o executable

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Message.o: Message.cpp Message.h
	$(CC) $(CFLAGS) Message.cpp

Security.o: Security.cpp
	$(CC) $(CFLAGS) Security.cpp

UDP/Socket.o: UDP/Socket.cpp UDP/Socket.h
	$(CC) $(CFLAGS) UDP/Socket.cpp

UDP/UDPCommands.o: UDP/UDPCommands.cpp UDP/UDPCommands.h
	$(CC) $(CFLAGS) UDP/UDPCommands.cpp

UDP/UDPPacket.o: UDP/UDPPacket.h UDP/UDPPacket.cpp
	$(CC) $(CFLAGS) UDP/UDPPacket.cpp

UDP/UDPPacket.o: UDP/UDPPacket.cpp UDP/UDPPacket.h
	$(CC) $(CFLAGS) UDP/UDPPacket.cpp

Server/Server.o: Server/Server.h Server/Server.cpp
	$(CC) $(CFLAGS) Server/Server.cpp

Server/ThreadPool.o: Server/ThreadPool.h Server/ThreadPool.cpp
	$(CC) $(CFLAGS) Server/ThreadPool.cpp

Server/UserHandler.o: Server/UserHandler.h Server/UserHandler.cpp
	$(CC) $(CFLAGS) Server/UserHandler.cpp

Client/Client.o: Client/Client.h Client/Client.cpp
	$(CC) $(CFLAGS) Client/Client.cpp

clean:
	rm -rf *o executable