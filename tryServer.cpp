#include "server.h"
#include "database_funcs.h"
#include "handler.h"

#include <pthread.h>

#define CREATE_TABLE "createTables.txt"
#define DROP_TABLE "dropTables.txt"
#define DB_INFO "dbname=MATCH_ENGINE user=postgres password=passw0rd"

int main(){
    try{
        connection *C;
        connectDB(DB_INFO, &C);
        dropTable(DROP_TABLE,C);
        createTable(CREATE_TABLE,C);
        disconnectDB(C);
        delete C;

        Server server(PORT);
        while(true){
            std::string ip_addr;
            cout<<"Waiting for connection"<<endl;
            int client_fd = server.accept_connection(&ip_addr);
            cout<<"Successfully connected with the client from "<<ip_addr<<endl;
            cout<<client_fd<<endl;
            pthread_t thread;
            pthread_create(&thread,NULL,xmlHandler,&client_fd);
        }
        
    }catch(const exception &e){
        cerr<<e.what()<<endl;
        return 1;
    }
}