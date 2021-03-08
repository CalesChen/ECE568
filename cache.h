#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <utility>
#include <iostream>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "helper.h"
#include "response.h"
#include "request_info.h"

class Cache{
	public:
		int capacity;
		std::ofstream& file;
		std::unordered_map<std::string, std::list<std::pair<std::string, Response> >::iterator> cacheMap;
		std::list<std::pair<std::string, Response> > cacheList;

		//Cache(int capacity):capacity(capacity), file(NULL){}
		Cache(int capacity, std::ofstream& logFile):capacity(capacity), file(logFile){}
		Response* getCache(std::string url, int oriServer_fd, int thread_id, request_info* request);
		void putCache(Response response, std::string url, int thread_id);
		bool revalidate(Response * response,int oriServer_fd, std::string request);

};