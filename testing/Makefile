CC=g++
CFLAGS=-std=c++11
EXTRAFLAGS=-lpqxx -lpq -lpthread -lxerces-c

all: client

client: tryClient.cpp client.cpp client.h socket_funcs.h socket_funcs.cpp
	$(CC) $(CFLAGS)  -o client tryClient.cpp client.cpp socket_funcs.cpp $(EXTRAFLAGS)


.PHONY:
	clean
clean:
	rm -rf *.o client