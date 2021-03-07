#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <fstream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
//#include "parameter.h"
//#include "helper.h"
//#include "request_info.h"
#include "cache.h"
//#include "response.h"
//server_start   client_start   server_recv
class Proxy{
	public:
		char * portNum;

		Proxy(char * port) : portNum(port){}
		void handleProxy(Cache* cache);
		static void * handleReq(void * parameter);
		//void* handleReq(int server_fd,int client_fd, int thread_id, std::string ip,Cache* cache);
		static int connectOriginalServer(request_info * parsedRequest);
		static std::string getCurrTime();
		//****************************Added New Code**********************//
		static void request_from_server();
    	//int recv_message(int socket_fd, vector<char> * buffer, bool isChunk);
    	//int recive(int socket_fd, void * buffer, size_t size, bool isChunk);
    	static void handleConnect(int socket_fd, int server_fd, int id);
    	//void Proxy::handlePost(int client_fd, int server_fd, int id, int len, vector<char> * request, const char * host);
		static void handlePost(int client_fd, int server_fd, int thread_id, request_info * request);
		static void handleGet(int client_fd, int server_fd, int thread_id, request_info * request, Cache* cache);
		static void ServerGet(int client_fd, int server_fd, int thread_id, request_info * request, Cache * cache);
    	//void Proxy::handleGet(int client_fd, int server_fd, int id, int len, vector<char> * request, const char * host, Cache* cache);
    	//void Proxy::ServerGet(int client_fd, int server_fd, int id, int len, vector<char> * request, const char * host_url);
};

class Parameter{
    public:
    int server_fd;
    int client_fd;
    int thread_id;
    std::string ip;
    Cache * cache;

    Parameter(int s_fd, int c_fd, int thread_id, std::string ip, Cache * cache):
    server_fd(s_fd), client_fd(c_fd), thread_id(thread_id), ip(ip), cache(cache){}
};
