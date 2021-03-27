#ifndef __DATABASE_FUNCS__
#define __DATABASE_FUNCS__

#include <iostream>
#include <fstream>
#include <pqxx/pqxx>

#include "MyException.h"

using namespace std;
using namespace pqxx;

void connectDB(string info, connection **C);
void disconnectDB(connection *C);
void executeQuery(string query, connection *C);
void dropTable(const char *fileName, connection *C);
void createTable(const char *fileName, connection *C);


#endif