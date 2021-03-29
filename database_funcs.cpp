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