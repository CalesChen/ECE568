#include "database_funcs.h"


void connectDB(string info, connection **C){
    try{
        *C = new connection(info);
        if ((*C)->is_open()) {
            cout << "Opened database successfully: " << (*C)->dbname() << endl;
        } else {
            throw MyException("Error when opening database","");
        }
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return;
    }
}

void disconnectDB(connection *C){
    if(C->is_open()){
        string dbname = C->dbname();
        C->disconnect();
        cout << "Closed database successfully: " << dbname << endl;
    }
}

void executeQuery(string query, connection *C){
    work W(*C);
    W.exec(query);
    W.commit();
}

void dropTable(const char *fileName, connection *C){
    string query,tableName;
    ifstream file(fileName);  
    if(file.is_open()){
        while(getline(file,tableName)){
            query.append("DROP TABLE IF EXISTS "+tableName+" CASCADE;");
        }
        executeQuery(query,C);
        cout<<"Successfully dropped possible tables from last running"<<endl;
        file.close();
        return;
    }else{
        cerr<<"Unable to open the file for droping tables!"<<endl;
        return;
    }
}

void createTable(const char *fileName, connection *C){
    string sqlStatement, line;
    ifstream file(fileName);

    if(file.is_open()){
        while(getline(file,line)){
        sqlStatement+=line;
        }
        file.close();
        executeQuery(sqlStatement,C);
        cout<<"Successfully initiate tables for the database"<<endl;
        return;
    }else{
        cerr<<"Unable to open the file for creating tables!"<<endl;
        return;
    }
}

bool accountExist(connection *C, long account_id){
    nontransaction N(*C);
    stringstream sqlStatement;
    sqlStatement << "SELECT * FROM ACCOUNT WHERE ACCOUNT_ID="
    << N.quote(account_id) << ";";
    result r(N.exec(sqlStatement.str()));
    return r.size() != 0;
}

bool createAccount(connection *C,long account_id, double balance){
    work W(*C);
    stringstream sqlStatement;
    sqlStatement << "INSERT INTO ACCOUNT (ACCOUNT_ID, BALANCE) "
    <<"VALUES ("<<W.quote(account_id)<<", "<<W.quote(balance)<<");";
    try{
        W.exec(sqlStatement.str());
        W.commit();
    }catch(const exception &e){
        cerr<<e.what()<<endl;
        W.abort();
        return false;
    }
    return true;
}

void createSymbol(connection *C, string symbol_name){
    work W(*C);
    stringstream sqlStatement;
    sqlStatement<<"INSERT INTO SYMBOL (NAME) "
    <<"VALUES ("<<W.quote(symbol_name)<<");";
    try{
        W.exec(sqlStatement.str());
        W.commit();
    }catch(const exception &e){
        cerr<<e.what()<<endl;
        W.abort();
        return;
    }
    return;
}

bool addPosition(connection *C,string symbol_name, long account_id, double share){
    createSymbol(C,symbol_name);

    work W(*C);

    stringstream sqlStatement;
    sqlStatement<<"INSERT INTO POSITION "
    <<"(SYMBOL_NAME, ACCOUNT_ID, AMOUNT) "
    <<"VALUES ("<<W.quote(symbol_name)<<", "
    <<W.quote(account_id)<<", "<<W.quote(share)<<")"
    <<"ON CONFLICT ON CONSTRAINT POSITION_PK "
    <<"DO UPDATE SET AMOUNT = POSITION.AMOUNT + "
    <<W.quote(share)<<";";

    W.exec(sqlStatement.str());
    W.commit();

    return true;
}

bool transactionExist(connection *C, long trans_id){
    nontransaction N(*C);
    stringstream sqlStatement;
    sqlStatement << "SELECT TRANS_ID FROM TRANSACTIONS WHERE TRANS_ID="
    << N.quote(trans_id) << ";";
    result r(N.exec(sqlStatement.str()));
    return r.size() != 0;
}

string queryTrans(connection *C, long trans_id){
    nontransaction N(*C);

    stringstream sqlStatement_tran;
    sqlStatement_tran << "SELECT STATUS, AMOUNT FROM TRANSACTIONS WHERE TRANS_ID="
    << N.quote(trans_id) << ";";
    result rT(N.exec(sqlStatement_tran.str()));
    assert(rT.size()==1);
    
    stringstream sqlStatement_deal;
    sqlStatement_deal << "SELECT PRICE, AMOUNT, TIME FROM DEAL WHERE TRANS_ID="
    << N.quote(trans_id) << ";";
    result rD(N.exec(sqlStatement_deal.str()));
    vector<double> deal_shares, deal_price;
    vector<long> deal_time;
    for(result::iterator iD = rD.begin();iD!=rD.end();iD++){
        deal_price.push_back(iD[0].as<double>());
        deal_shares.push_back(iD[1].as<double>());
        deal_time.push_back(iD[2].as<long>());
    }

    result::iterator iT = rT.begin();
    string status = iT[0].as<string>();
    double trans_share = iT[1].as<double>();
    Result res;
    if(status=="OPEN"){
        return res.queryResult(trans_id,trans_share,deal_shares,deal_price,deal_time);
    }else if(status=="CANCELED"){
        stringstream sqlStatement_cancel;
        sqlStatement_cancel << "SELECT CANCEL_TIME FROM CANCELTIME WHERE TRANS_ID="
        << N.quote(trans_id) << ";";
        result rC(N.exec(sqlStatement_cancel.str()));
        long cancel_time = rC.begin()[0].as<long>();
        return res.queryResult(trans_id,trans_share,cancel_time,deal_shares,deal_price,deal_time);
    }else{
        cerr<<"A transaction with neither open nor canceled status: "<<status<<endl;
        return "";
    }

}

/*
    The function will return true if we need further match. 
    will return false if we do not need to match.
*/
bool matchOrderBuyer(connection *C, long buyer_tran_id, string & sym, double amount, double price, long buyer_id){
    // This is for buyer
    // Select eligible Transaction ID, price, amount, from TRANSACTIONS 
    // Where TRANSACTIONS.status = "OPEN" and TRANSACTIONS.SYMBOL_NAME="sym" and TRANSACTIONS.LIMIT_PRICE < 0 and TRANSACTIONS.LIMIT_PRICE > (-price) 
    // Order By price ASC
    work W(*C);
    stringstream ss;
    ss<<"SELECT TRANS_ID, SYMBOL_NAME, ACCOUNT_ID, LIMIT_PRICE, AMOUNT, TIME";
    ss<<"From TRANSACTIONS";
    ss<<"Where TRANSACTIONS.status="<<W.quote("OPEN")<< " and TRANSACTIONS.SYMBOL_NAME="<<W.quote(sym);
    ss<<" and TRANSACTIONS.AMOUNT < 0 and TRANSACTIONS.LIMIT_PRICE <= "<<(price);
    ss<<" Order By LIMIT_PRICE ASC, TIME ASC;";
    result r(W.exec(ss.str()));
    auto c = r.begin();
    if(c == r.end()){
        return false;
    }
    long seller_id=c[2].as<long>(), seller_tran_id = c[0].as<long>();
    double buyerAmountRemain=amount+c[4].as<double>(), shares=-c[4].as<double>();
    string sym = c[1].as<string>();
    if(buyerAmountRemain <= 0){
        //seller sell part of shares
        double sellerAmountRemain = buyerAmountRemain;
        double totalPrice = amount*price;
        updateTransactionStatus(W,seller_tran_id,buyer_tran_id,sellerAmountRemain,buyer_id,seller_id,sym,amount,totalPrice);
        //return false to continue trying to match more trans
        return false;
    }else if(buyerAmountRemain > 0){
        // Seller sells all of his shares.
        double totalPrice = shares*price;
        updateTransactionStatus(W,buyer_tran_id,seller_tran_id,buyerAmountRemain,buyer_id,seller_id,sym,shares,totalPrice);
        return true;
        // Return true is the buyer has done his transaction
    }
}

/*
    The function will return true if we need further match. 
    will return false if we do not need to match.
*/
bool matchOrderSeller(connection *C, long seller_tran_id, string & sym, double amount, double price, long seller_id){
    // This is for seller
    // Select eligible Transaction ID, price, amount, from TRANSACTIONS 
    // Where TRANSACTIONS.status = "OPEN" and TRANSACTIONS.SYMBOL_NAME="sym" and TRANSACTIONS.LIMIT_PRICE < 0 and TRANSACTIONS.LIMIT_PRICE > (-price) 
    // Order By price ASC
    work W(*C);
    stringstream ss;
    ss<<"SELECT TRANS_ID, SYMBOL_NAME, ACCOUNT_ID, LIMIT_PRICE, AMOUNT, TIME";
    ss<<"From TRANSACTIONS";
    ss<<"Where TRANSACTIONS.status="<<W.quote("OPEN")<< " and TRANSACTIONS.SYMBOL_NAME="<<W.quote(sym);
    ss<<" and TRANSACTIONS.AMOUNT > 0 and TRANSACTIONS.LIMIT_PRICE >= "<<(price);
    ss<<" Order By LIMIT_PRICE DESC, TIME ASC;";
    result r(W.exec(ss.str()));
    auto c = r.begin();
    if(c == r.end()){
        return false;
    }
    long buyer_id=c[2].as<long>(), buyer_tran_id = c[0].as<long>();
    double sellerAmountRemain=amount+c[4].as<double>(), shares=c[4].as<double>(), dealPrice = c[3].as<double>();
    string sym = c[1].as<string>();
    if(sellerAmountRemain < 0){
        //seller sell part of shares
        //buyerremain = 0 sellerremain<0,
        double totalPrice = shares*dealPrice;
        updateTransactionStatus(W,seller_tran_id,buyer_tran_id,sellerAmountRemain,seller_id,seller_id,sym,shares,totalPrice);
        //return false to continue trying to match more trans
        return true;
    }else if(sellerAmountRemain >= 0){
        // seller sells all shares.
        double buyerAmountRemain = sellerAmountRemain;
        double totalPrice = amount*dealPrice;
        updateTransactionStatus(W,buyer_tran_id,seller_tran_id,buyerAmountRemain,seller_id,seller_id,sym,amount,totalPrice);
        return false;
        // Return true is the seller has done his transaction
    }
}

void updateTransactionStatus(work & W, long open_trans_id, long close_trans_id, double amountRemain, long buyer_id, long seller_id, string & sym, double shares, double balance){
    while(true){
        try{
            updateTransactionStatus(W, open_trans_id, amountRemain, "OPEN");
            updateTransactionStatus(W, close_trans_id, 0, "OPEN");
            updateAccountShareAndMoney(W, buyer_id, seller_id, sym, shares, balance);
            //TODO :insert a record for this deal
            W.commit();
            break;
        }catch(exception & e){
            cerr<<e.what()<<endl;
            W.abort();
        }
    }
    
}
void updateTransactionStatus(work & W, long trans_id, double amount, string status){
    stringstream ss;
    ss<<"UPDATE TRANSACTIONS ";
    ss<<"SET STATUS="<<W.quote(status)<<" AMOUNT="<<amount;
    ss<<" Where TRANSACTIONS.TRANS_ID="<<trans_id<<";";
    W.exec(ss.str());
}
// Buyer got Shares and Seller got balance.
void updateAccountShareAndMoney(work & W, long buyer_id, long seller_id, string & sym, double shares, double balance){
    stringstream ss;
    // Update Position
    ss<<"UPDATE POSITION ";
    ss<<"SET AMOUNT=POSITION.AMOUNT+"<<shares;
    ss<<" Where POSITION.SYMBOL_NAME="<<sym<<" And POSTION.ACCOUNT_ID="<<buyer_id<<";";
    
    // Update Balance

    ss<<"UPDATE ACCOUNT ";
    ss<<"SET BALANCE=ACCOUNT.BALANCE+"<<balance;
    ss<<" Where ACCOUNT.ACCOUNT_ID="<<seller_id<<";";

    W.exec(ss.str());
}