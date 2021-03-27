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