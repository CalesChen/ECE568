#include "server.h"

int main(){
    try{
        Server server(PORT);
        std::string ip_addr;
        cout<<"Waiting for connection"<<endl;
        int browser_fd = server.accept_connection(&ip_addr);
        cout<<"Successfully connected to "<<ip_addr<<endl;
    }catch(std::exception e){
        cerr<<e.what()<<endl;
    }
}