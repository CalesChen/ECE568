#ifndef __DATABASE_FUNCS_H__
#define __DATABASE_FUNCS_H__

#include <iostream>
#include <string>
#include <fstream>
#include <pqxx/pqxx>

#include "MyException.h"
#include "assert.h"
#include "result.h"

using namespace std;
using namespace pqxx;

void connectDB(string info, connection **C);
void disconnectDB(connection *C);
void executeQuery(string query, connection *C);
void dropTable(const char *fileName, connection *C);
void createTable(const char *fileName, connection *C);
// if the account exist
bool accountExist(connection *C, long account_id);
//create an account
bool createAccount(connection *C,long account_id, double balance);
//record symbol_name in table SYMBOL
void createSymbol(connection *C, string symbol_name);
//create or add <share> shares of <symbol_name> into account <account_id>
bool addPosition(connection *C,string symbol_name, long account_id, double share);
bool transactionExist(connection *C, long trans_id);
string queryTrans(connection *C,long trans_id, long account_id);
#endif