#include "server.h"
#include "database_funcs.h"

#define CREATE_TABLE "createTables.txt"
#define DROP_TABLE "dropTables.txt"
#define DB_INFO "dbname=MATCH_ENGINE user=postgres password=passw0rd"

int main(){
    try{
        Server server(PORT);
        std::string ip_addr;
        cout<<"Waiting for connection"<<endl;
        int browser_fd = server.accept_connection(&ip_addr);
        cout<<"Successfully connected to "<<ip_addr<<endl;

        connection *C;
        connectDB(DB_INFO, &C);
        disconnectDB(C);
        
    }catch(std::exception e){
        cerr<<e.what()<<endl;
        return 1;
    }
}