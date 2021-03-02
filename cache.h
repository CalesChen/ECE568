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
#include <stdlib.h>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "helper.cpp"
#include "response.h"
#include "proxy.h"

class Cache{
	public:
		int capacity;
		std::unordered_map<std::string, std::list<std::pair<std::string, Response> >::iterator> cacheMap;
		std::list<std::pair<std::string, Response> > cacheList;

		Cache(int capacity):capacity(capacity){}


		Response getCache(std::string url, int oriServer_fd, int thread_id, std::string request);
		void putCache(Response response, std::string url, int thread_id);

};