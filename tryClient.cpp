#include "client.h"

int main(){
    try{
        Client client("127.0.0.1",PORT);
        cout<<"Successfully connected"<<endl;
        string myxml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"15\"><order sym=\"SYM\" amount=\"50\" limit=\"1\"/></transactions>";
        sendString(client.socket_fd,myxml);
        string ans = recvString(client.socket_fd);
        cout<<ans<<endl;
    }catch(const exception e){
        cerr<<e.what()<<endl;
    }
}