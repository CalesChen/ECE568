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
        // createSymbol(C,"ABC");
        // createAccount(C,12,100);
        // long trans_id = insertTrans(C,12,"ABC",2,3);
        // cout<<trans_id<<endl;
        disconnectDB(C);
        delete C;

        
        pthread_mutex_t mutex;
        Server server(PORT);
        while(true){
            try {
                XMLPlatformUtils::Initialize();
            }
            catch (const XMLException& toCatch)
            {
                std::cerr << XMLString::transcode(toCatch.getMessage());
                throw new exception();
            }
            std::string ip_addr;
            cout<<"Waiting for connection"<<endl;
            pthread_mutex_lock(&mutex);
            int client_fd = server.accept_connection(&ip_addr);
            Parameter * p = new Parameter(client_fd);
            cout<<"Successfully connected with the client from "<<ip_addr<<endl;
            cout<<client_fd<<endl;
            pthread_mutex_unlock(&mutex);

            pthread_t thread;
            pthread_create(&thread,NULL,xmlHandler,p);
        }
        
    }catch(const exception &e){
        cerr<<e.what()<<endl;
        return 1;
    }
}
