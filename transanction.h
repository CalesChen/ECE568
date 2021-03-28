#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace xercesc;

class Order{
    public:
        int account_id;
        string sym;
        int amount;
        double limit;

    Order(string sym, int amount, double limit):sym(sym), amount(amount),limit(limit){}
};

class transaction{
    public:
    vector<Order> orders;
    vector<int> querys;
    vector<int> cancels;

    transaction();
};