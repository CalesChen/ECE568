#ifndef __CREATE_H__
#define __CREATE_H__
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

class SymbolCreate{
    public:
        string symbol;
        vector<long> account_id;
        vector<string> orgin_id;
        vector<double> num;
};
class Create{
    public:
        vector<long> account_id;
        vector<string> origin_id;
        vector<double> account_balance;
        vector<SymbolCreate> symbol;

    Create(string myxml);
};

#endif