#include "errorMSG.h"

string ErrorMSG::createAccountErrorMSG(long id, string & msg){
    stringstream ss;
    ss<<"<error id=\""<<id<<"\">"<<msg<<"</error>";
    return ss.str();    
}

string ErrorMSG::createSymbolErrorMSG(long id, string sym, string & msg){
    stringstream ss;
    ss<<"<error sym=\"" << sym <<"\" id=\""<<id<<"\">"<<msg<<"</error>";
    return ss.str();
}

string ErrorMSG::transactionEmptyErrorMSG(long Acc_id, string & msg){
    stringstream ss;
    ss<<"<error id=\""<<Acc_id<<"\">"<<msg<<"</error>";
    return ss.str();
}

string ErrorMSG::orderErrorMSG(string sym, double amount, double limit, string & msg){
    stringstream ss;
    ss<<"<error sym=\""<<sym<<"\" amount=\""<<amount<<"\" limit=\""<<limit<<"\">"<<msg<<"</error>";
    return ss.str();
}

string ErrorMSG::queryErrorMSG(long tran_id, string & msg){
    stringstream ss;
    ss<<"<error id=\""<<tran_id<<"\">"<<msg<<"</error>";
    return ss.str();
}

string ErrorMSG::cancelErrorMSG(long tran_id, string & msg){
    stringstream ss;
    ss<<"<error id=\""<<tran_id<<"\">"<<msg<<"</error>";
    return ss.str();
}

