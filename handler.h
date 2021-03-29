#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "MyException.h"
#include "socket_funcs.h"
#include "create.h"
#include "transanction.h"

using namespace std;

void *xmlHandler(void *client_fd);

#endif