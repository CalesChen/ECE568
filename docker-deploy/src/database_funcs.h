#ifndef __DATABASE_FUNCS_H__
#define __DATABASE_FUNCS_H__

#include <iostream>
#include <string>
#include <fstream>
#include <pqxx/pqxx>
#include <ctime>

#include "MyException.h"
#include "assert.h"
#include "result.h"
#include "create.h"
#include "errorMSG.h"

#define CREATE_TABLE "createTables.txt"
#define DROP_TABLE "dropTables.txt"
#define DB_INFO "dbname=MATCH_ENGINE user=postgres password=passw0rd host=localhost port=5432"

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
double getAccountBalance(connection *C, long account_id);
void setAccountBalance(connection *C, long account_id, double remain);
//record symbol_name in table SYMBOL
void createSymbol(connection *C, string symbol_name);
//create or add <share> shares of <symbol_name> into account <account_id>
bool addPosition(connection *C,string symbol_name, long account_id, double share);
bool transactionExist(connection *C, long trans_id);
string queryTrans(connection *C,long trans_id);
long getCurrTime();

// Match Order Related Function
bool matchOrderBuyer(connection *C, long buyer_tran_id);
bool matchOrderSeller(connection *C, long seller_tran_id);
bool updateTransactionStatus(work & W, long open_trans_id, long close_trans_id, double amountRemain, long buyer_id, long seller_id, string & sym, double shares, double balance);
void updateTransactionStatus(work & W, long trans_id, double amount, string status);
void updateAccountShareAndMoney(work & W, long buyer_id, long seller_id, string & sym, double shares, double balance);
//order tag related function
long insertTrans(work &W, long account_id, string sym, long amount, double limit_price);
string processOrder(connection *C, long account_id,string sym, long amount, double limit_price);
string cancelResult(connection * C, long trans_id);
#endif