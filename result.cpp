#include "result.h"

string Result::createAccountResult(long acc_id){
    stringstream ss;
    ss<<"<created id=\""<<acc_id<<"\"/>";
    return ss.str();
}
string Result::createSymbolResult(string & sym, long acc_id){
    stringstream ss;
    ss<<"<created sym=\""<<sym<<"\" id=\""<<acc_id<<"\"/>";
    return ss.str();
}
string Result::openResult(string & sym, double amount, double limit, long tran_id){
    stringstream ss;
    ss<<"<opened sym=\""<<sym<<"\" amount=\""<<amount<<"\" limit=\""<<limit<<"\" id=\""<<tran_id<<"\"/>";
    return ss.str();
}
string Result::queryResult(long tran_id, double openShares, vector<double> & shares, vector<double> & price, vector<long> & time){
    stringstream ss;
    ss<<"<status id=\""<<tran_id<<"\">";
    ss<<"<open shares=\""<<openShares<<"\"/>";
    for(int i = 0 ; i < price.size() ; i++){
        ss<<"<executed shares=\""<<shares[i]<<"\" price=\""<<price[i]<<"\" time=\""<<time[i]<<"\"/>";
    }
    ss<<"</status>";
    return ss.str();
}
string Result::queryResult(long tran_id, double canceledShares, long canceldTime, vector<double> & shares, vector<double> & price, vector<long> & time){
    stringstream ss;
    ss<<"<status id=\""<<tran_id<<"\">";
    ss<<"<canceled shares=\""<<canceledShares<<"\" time=\""<<canceldTime<<"\"/>";
    for(int i = 0 ; i < price.size() ; i++){
        ss<<"<executed shares=\""<<shares[i]<<"\" price=\""<<price[i]<<"\" time=\""<<time[i]<<"\"/>";
    }
    ss<<"</status>";
    return ss.str();
}
string Result::cancelResult(long tran_id, double canceledShare, long canceldTime, vector<double> & shares, vector<double> & price, vector<long> & time){
    stringstream ss;
    ss<<"<canceled id=\""<<tran_id<<"\">";
    ss<<"<canceled shares=\""<<canceledShare<<"\" time=\""<<canceldTime<<"\"/>";
    for(int i = 0 ; i < price.size() ; i++){
        ss<<"<executed shares=\""<<shares[i]<<"\" price=\""<<price[i]<<"\" time=\""<<time[i]<<"\"/>";
    }
    ss<<"</status>";
    return ss.str();
}