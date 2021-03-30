#include "server.h"
#include "database_funcs.h"
#include "handler.h"

#include <pthread.h>

int main(){
    try{
        connection *C;
        connectDB(DB_INFO, &C);
        //remove dropTable() when deploy, 
        //exist while developing to ensure the database empty when started
        dropTable(DROP_TABLE,C);
        createTable(CREATE_TABLE,C);
        disconnectDB(C);
        delete C;
        for(int i =0;i<10;i++){
            cout<<getCurrTime()<<endl;
            sleep(2);
        }

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