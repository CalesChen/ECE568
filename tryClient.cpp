#include "client.h"

int main(){
    try{
        Client client("127.0.0.1",PORT);
        cout<<"Successfully connected"<<endl;
        sendString(client.socket_fd,"Hi, I'm client!");
        string ans = recvString(client.socket_fd);
        cout<<ans<<endl;
    }catch(const exception e){
        cerr<<e.what()<<endl;
    }
}