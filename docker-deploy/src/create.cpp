#include "create.h"
#define CREATE_TAG "create"




    Create::Create(string myxml){
        try {
            XMLPlatformUtils::Initialize();
        }
        catch (const XMLException& toCatch)
        {
            std::cerr << XMLString::transcode(toCatch.getMessage());
            throw new exception();
        }

        XercesDOMParser * parser = new XercesDOMParser();
        ErrorHandler * errHandler = new HandlerBase();
        parser->setErrorHandler(errHandler);
        cout<<"parse success"<<endl;

        xercesc::MemBufInputSource * myxml_buf = new xercesc::MemBufInputSource((XMLByte *)(myxml.c_str()) , myxml.size(), "myxml(in memory)");

        try
        {
            parser->parse(*myxml_buf);
        }
        catch (const XMLException& toCatch)
        {
            std::cerr << XMLString::transcode(toCatch.getMessage())<<"0";
            throw new exception();
        }
        catch (const DOMException& toCatch)
        {
            std::cerr << XMLString::transcode(toCatch.getMessage())<<"1";
            throw new exception();
        }
        catch (const SAXException& toCatch)
        {
            std::cerr << XMLString::transcode(toCatch.getMessage())<<"2";
            throw new exception();
        }

        DOMDocument * doc = parser->getDocument();
        DOMElement * root = doc->getDocumentElement();
        cout<<XMLString::transcode(root->getNodeName())<<endl;

        DOMNodeList * account = root->getElementsByTagName(XMLString::transcode("account"));
        for(int i = 0 ; i < account->getLength() ; i++){
            DOMNode * temp = account->item(i);
            if(temp->getParentNode() != root){
                continue;
            }
            //temp->getNamedItem(XMLString::transcode("id"))->
            cout<<XMLString::transcode(temp->getNodeName())<<endl;
            string id = XMLString::transcode(temp->getAttributes()->getNamedItem(XMLString::transcode("id"))->getNodeValue());
            string balance = XMLString::transcode(temp->getAttributes()->getNamedItem(XMLString::transcode("balance"))->getNodeValue());
            long id_int = atol(id.c_str());
            double balance_int = atof(balance.c_str());
            //cout<<id_int + 3<<endl;
            //cout<<balance_int + id_int<<endl;
            this->account_balance.push_back(balance_int);
            this->account_id.push_back(id_int);
            this->origin_id.push_back(id);
            //cout<<XMLString::transcode(account->item(i)->get (XMLString::transcode("id")))<<endl;
        }

        DOMNodeList * symbol = root->getElementsByTagName(XMLString::transcode("symbol"));
        cout<<XMLString::transcode(root->getNodeName())<<endl;

        for(int i = 0 ;  i< symbol->getLength();i++){
            SymbolCreate tempToAdd;
            DOMNode * temp = symbol->item(i);
            cout<<XMLString::transcode(temp->getNodeName())<<endl;

            string sym = XMLString::transcode(temp->getAttributes()->getNamedItem(XMLString::transcode("sym"))->getNodeValue());
            cout<<sym<<endl;
            tempToAdd.symbol = sym;

            DOMElement * temp1 = static_cast<DOMElement *>(temp);
            DOMNodeList * sym_acc = temp1->getElementsByTagName(XMLString::transcode("account"));
            cout<<sym_acc->getLength()<<endl;
            for(int j = 0 ; j < sym_acc->getLength();j++){
                DOMNode * temp_child = sym_acc->item(j);
                cout<<XMLString::transcode(temp_child->getNodeName())<<endl;
                string id = XMLString::transcode(temp_child->getAttributes()->getNamedItem(XMLString::transcode("id"))->getNodeValue());
                //string balance = XMLString::transcode(temp_child->getAttributes()->getNamedItem(XMLString::transcode("balance"))->getNodeValue());
                string num = XMLString::transcode(temp_child->getTextContent());
                //int id_int = atoi(id.c_str());
                //int balance_int = atoi(balance.c_str());
                //int num_int = atoi(num.c_str());
                cout<<id<<endl<<endl<<num<<endl;
                
                tempToAdd.account_id.push_back(atol(id.c_str()));
                tempToAdd.orgin_id.push_back(id);
                tempToAdd.num.push_back(atof(num.c_str()));
            }
            this->symbol.push_back(tempToAdd);
        }
        delete parser;
        delete errHandler;
        delete myxml_buf;
        XMLPlatformUtils::Terminate();
    }