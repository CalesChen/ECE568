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
        cout << "Opened database successfully: " << C->dbname() << endl;
        C->disconnect();
    }
}

void executeQuery(string query, connection *C){
    work W(*C);
    W.exec(query);
    W.commit();
}

void dropTable(const char *fileName, connection *C){
    string tableName;
    ifstream file(fileName);

    if(file.is_open()){
      while(getline(file,tableName)){
        string query = "DROP TABLE IF EXISTS "+tableName+" CASCADE;";
        executeQuery(query,C);
      }
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

    return;
  }else{
    cerr<<"Unable to open the file for creating tables!"<<endl;
    return;
  }
}