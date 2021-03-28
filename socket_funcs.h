#ifndef __SOCKET_FUNCS_H__
#define __SOCKET_FUNCS_H__

#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>

#include "MyException.h"

using namespace std;

void sendString(int fd, string msg);
string recvString(int fd);

#endif