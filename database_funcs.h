#ifndef __DATABASE_FUNCS_H__
#define __DATABASE_FUNCS_H__

#include <iostream>
#include <string>
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
// if the account exist
bool accountExist();
//create an account
void createAccount(long account_id, double balance);
//create or add <share> shares of <symbol_name> into account <account_id>
void addPosition(string symbol_name, long account_id, int share);

void queryTrans(long trans_id, long account_id);
#endif