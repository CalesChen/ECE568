#ifndef __RESULT_H__
#define __RESULT_H__

#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <sstream>

using namespace std;
class Result{
    public:
    string createAccountResult(long acc_id);
    string createSymbolResult(string & sym, long acc_id);
    string openResult(string & sym, double amount, double limit, long acc_id);
    string queryResult(long tran_id, double openShares, vector<double> & shares, vector<double> & price, vector<long> & time);
    string queryResult(long tran_id, double openShares, long canceldTime, vector<double> & shares, vector<double> & price, vector<long> & time);
    string cancelResult(long tran_id, double canceledShare, long canceldTime, vector<double> & shares, vector<double> & price, vector<long> & time);
    //string cancelResult(long tran_id, double canceled);
};

#endif