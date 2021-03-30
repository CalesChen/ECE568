#ifndef __SERVER_H__
#define __SERVER_H__

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

class Server{
public:
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname;
    const char *port;
    Server(const char *port):status(0),socket_fd(0),host_info_list(NULL),hostname(NULL),port(port){
        init_serverSocket();
    }
    ~Server(){
        if(socket_fd!=0){
            close(socket_fd);
        }
        if(host_info_list!=NULL){
            free(host_info_list);
        }
    }
    void init_serverSocket();
    int accept_connection(string *ip_addr);
    string getHostPortInfo();
};

#endif