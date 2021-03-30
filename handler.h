#ifndef __HANDLER_H__
#define __HANDLER_H__

#include <ctype.h>
#include "MyException.h"
#include "socket_funcs.h"
#include "create.h"
#include "transanction.h"
#include "errorMSG.h"
#include "database_funcs.h"

using namespace std;

bool checkName(string & sym);
void *xmlHandler(void *client_fd);

#endif