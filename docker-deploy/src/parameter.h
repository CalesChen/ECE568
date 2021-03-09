#include <string.h>
#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include "cache.h"

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