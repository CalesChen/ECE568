#include "server.h"
#include "database_funcs.h"

#define CREATE_TABLE "createTables.txt"
#define DROP_TABLE "dropTables.txt"
#define DB_INFO "dbname=MATCH_ENGINE user=postgres password=passw0rd"

int main(){
    try{
        connection *C;
        connectDB(DB_INFO, &C);
        dropTable(DROP_TABLE,C);
        createTable(CREATE_TABLE,C);
        
        Server server(PORT);
        std::string ip_addr;
        cout<<"Waiting for connection"<<endl;
        int browser_fd = server.accept_connection(&ip_addr);
        cout<<"Successfully connected to "<<ip_addr<<endl;

        disconnectDB(C);
    }catch(const exception &e){
        cerr<<e.what()<<endl;
        return 1;
    }
}