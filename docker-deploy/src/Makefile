CC=g++
CFLAGS=-O3
EXTRAFLAGS=-lpqxx -lpq -lpthread -lxerces-c

all: server

server: tryServer.cpp server.cpp server.h database_funcs.cpp database_funcs.h MyException.h handler.h handler.cpp socket_funcs.h socket_funcs.cpp result.h result.cpp create.h create.cpp transanction.h transanction.cpp
	$(CC) $(CFLAGS)  -o server tryServer.cpp server.cpp database_funcs.cpp socket_funcs.cpp handler.cpp errorMSG.cpp transanction.cpp create.cpp result.cpp $(EXTRAFLAGS)

.PHONY:
	clean
clean:
	rm -rf *.o server