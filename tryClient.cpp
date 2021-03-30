#include "client.h"

int main(){
    try{
        Client client("127.0.0.1",PORT);
        cout<<"Successfully connected"<<endl;
        string myxml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><create><account id=\"15\" balance=\"100\"/><account id=\"16\" balance=\"100\"/><symbol sym=\"SYM\"><account id=\"16\">100</account></symbol></create>";
        sendString(client.socket_fd,myxml);
        string ans = recvString(client.socket_fd);
        cout<<ans<<endl;
    }catch(const exception e){
        cerr<<e.what()<<endl;
    }
}