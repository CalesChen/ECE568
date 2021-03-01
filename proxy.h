#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>

//server_start   client_start   server_recv
class Proxy{
	public:
		char * portNum;

		void handleProxy();
		void handleReq(int server_fd,int client_fd, int thread_id, std::string ip);
		int connectOriginalServer(request_info * parsedRequest);
		std::string getCurrTime();
};