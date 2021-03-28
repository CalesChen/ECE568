#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>

#include "MyException.h"
#include "socket_funcs.h"

#define PORT "12345"
using namespace std;

class Client{
public:
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname;
    const char *port;
    Client(const char *hostname, const char *port):status(0),socket_fd(0),
        host_info_list(NULL),hostname(hostname),port(port){
        init_client();
    }
    ~Client(){
        if(socket_fd!=0){
            close(socket_fd);
        }
        if(host_info_list!=NULL){
            free(host_info_list);
        }
    }
    void init_client();
    string getHostPortInfo();
};

#endif