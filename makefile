CC=g++

ADDIT = -std=c++11 -Wl,--no-as-needed -pthread -ggdb -fpermissive
CFLAGS=-c $(ADDIT)

all: hello

hello: 	main.o 			\
	Misc/Security.o Misc/timer.o	\
	Payload/Message.o	\
	UDP/Socket.o UDP/Commands/UDPCommands.o UDP/UDPPacket.o UDP/Commands/ACKCommand.o \
	Server/Server.o Server/ThreadPool.o Server/UserHandler.o  \
	Client/Client.o
	$(CC) $(ADDIT) main.o Message.o ACKCommand.o \
		Socket.o UDPCommands.o UDPPacket.o \
		Server.o ThreadPool.o UserHandler.o  \
		Client.o timer.o Security.o -o executable

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Misc/Security.o: Misc/Security.cpp Misc/Security.h
	$(CC) $(CFLAGS) Misc/Security.cpp

Misc/timer.o: Misc/timer.cpp  Misc/timer.h
	$(CC) $(CFLAGS) Misc/timer.cpp

Payload/Message.o: Payload/Message.cpp Payload/Message.h
	$(CC) $(CFLAGS) Payload/Message.cpp

UDP/Socket.o: UDP/Socket.cpp UDP/Socket.h
	$(CC) $(CFLAGS) UDP/Socket.cpp

UDP/Commands/UDPCommands.o: UDP/Commands/UDPCommands.cpp UDP/Commands/UDPCommands.h
	$(CC) $(CFLAGS) UDP/Commands/UDPCommands.cpp

UDP/Commands/ACKCommand.o : UDP/Commands/ACKCommand.cpp UDP/Commands/ACKCommand.h
	$(CC) $(CFLAGS) UDP/Commands/ACKCommand.cpp

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
