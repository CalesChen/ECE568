#ifndef __ERRORMSG_H__
#define __ERRORMSG_H__

#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>

using namespace std;

class ErrorMSG{
    public:
        string createAccountErrorMSG(long id, string & msg);

        string createSymbolErrorMSG(long id, string sym, string & msg);

        string transactionEmptyErrorMSG(long Acc_id, string & msg);

        string orderErrorMSG(string sym, double amount, double limit, string & msg);

        string queryErrorMSG(long tran_id, string & msg);

        string cancelErrorMSG(long tran_id, string & msg);
};

#endif