CC=g++
CFLAGS=-O3
EXTRAFLAGS=-lpqxx -lpq -lpthread -lxerces-c

all: server client

server: tryServer.cpp server.cpp server.h database_funcs.cpp database_funcs.h MyException.h handler.h handler.cpp socket_funcs.h socket_funcs.cpp
	$(CC) $(CFLAGS)  -o server tryServer.cpp server.cpp database_funcs.cpp socket_funcs.cpp handler.cpp $(EXTRAFLAGS)
client: tryClient.cpp client.cpp client.h socket_funcs.h socket_funcs.cpp
	$(CC) $(CFLAGS)  -o client tryClient.cpp client.cpp socket_funcs.cpp $(EXTRAFLAGS)


.PHONY:
	clean
clean:
	rm -rf *.o server client