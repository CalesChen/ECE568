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
        vector<int> account_id;
        vector<int> num;
};
class create{
    public:
        vector<long> account_id;
        vector<int> account_balance;
        vector<SymbolCreate> symbol;

    create(string myxml);
};