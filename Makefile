all: server client

server: tryServer.cpp server.cpp
	g++ -g  -o server tryServer.cpp server.cpp 
client: tryClient.cpp client.cpp
	g++ -g  -o client tryClient.cpp client.cpp


.PHONY:
	clean
clean:
	rm -rf *.o server client