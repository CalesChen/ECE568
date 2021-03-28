#include "transanction.h"

class Transaction{
    public:
    int account_id;
    vector<Order> orders;
    vector<int> queries;
    vector<int> cancels;

    Transaction(string myxml){
        try{
            XMLPlatformUtils::Initialize();
        }catch(const XMLException& toCatch){
            cerr<<XMLString::transcode(toCatch.getMessage());
            throw new exception();
        }


        XercesDOMParser * parser = new XercesDOMParser();
        ErrorHandler * errHandler = new HandlerBase();
        parser->setErrorHandler(errHandler);
        cout<<"Parse Success"<<endl;

        xercesc::MemBufInputSource * myxml_buf = new xercesc::MemBufInputSource((XMLByte *)(myxml.c_str()) , myxml.size(), "myxml(in memory)");

        try{
            parser->parse(*myxml_buf);
        }catch (const XMLException& toCatch){
            std::cerr << XMLString::transcode(toCatch.getMessage());
            throw new exception();
        }catch (const DOMException& toCatch){
            std::cerr << XMLString::transcode(toCatch.getMessage());
            throw new exception();
        }catch (const SAXException& toCatch){
            std::cerr << XMLString::transcode(toCatch.getMessage());
            throw new exception();
        }

        DOMDocument * doc = parser->getDocument();
        DOMElement * root = doc->getDocumentElement();
        cout<<XMLString::transcode(root->getNodeName())<<endl;
        string ac_id = XMLString::transcode(root->getAttribute(XMLString::transcode("id")));
        cout<<"ac_id"<<ac_id<<endl;
        this->account_id = atoi(ac_id.c_str());
        DOMNodeList * order = root->getElementsByTagName(XMLString::transcode("order"));
        for(int i = 0 ; i < order->getLength();i++){
            DOMNode * temp = order->item(i);
            string sym = XMLString::transcode(temp->getAttributes()->getNamedItem(XMLString::transcode("sym"))->getNodeValue());
            string amount = XMLString::transcode(temp->getAttributes()->getNamedItem(XMLString::transcode("amount"))->getNodeValue());
            string limit = XMLString::transcode(temp->getAttributes()->getNamedItem(XMLString::transcode("limit"))->getNodeValue());
            Order order_temp(sym, atoi(amount.c_str()), atof(limit.c_str()));
            orders.push_back(order_temp);
            cout<<amount<<endl<<sym<<endl<<limit<<endl;
        }

        DOMNodeList * query = root->getElementsByTagName(XMLString::transcode("query"));
        cout<<query->getLength()<<endl;
        for(int i = 0 ; i < query->getLength();i++){
            DOMNode * temp = query->item(i);
            string id = XMLString::transcode(temp->getAttributes()->getNamedItem(XMLString::transcode("id"))->getNodeValue());
            queries.push_back(atoi(id.c_str()));
        }
        
        DOMNodeList * cancel = root->getElementsByTagName(XMLString::transcode("cancel"));
        for(int i = 0 ; i < cancel->getLength();i++){
            DOMNode * temp = cancel->item(i);
            string id = XMLString::transcode(temp->getAttributes()->getNamedItem(XMLString::transcode("id"))->getNodeValue());
            cancels.push_back(atoi(id.c_str()));
        }
        
        delete parser;
        delete errHandler;
        delete myxml_buf;
        XMLPlatformUtils::Terminate();
    }
};