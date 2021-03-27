CC=g++
CFLAGS=-O3
EXTRAFLAGS=-lpqxx -lpq

all: server client

server: tryServer.cpp server.h MyException.h database_funcs.h server.cpp database_funcs.cpp
	$(CC) $(CFLAGS)  -o server tryServer.cpp server.cpp database_funcs.cpp $(EXTRAFLAGS)
client: tryClient.cpp client.cpp client.h
	$(CC) $(CFLAGS)  -o client tryClient.cpp client.cpp $(EXTRAFLAGS)


.PHONY:
	clean
clean:
	rm -rf *.o server client