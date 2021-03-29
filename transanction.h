#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace xercesc;

class Order{
    public:
        long account_id;
        string sym;
        double amount;
        double limit;

    Order(string sym, int amount, double limit):sym(sym), amount(amount),limit(limit){}
};

class Transaction{
    public:
    long account_id;
    vector<Order> orders;
    vector<long> querys;
    vector<long> cancels;

    Transaction(string myxml);
};