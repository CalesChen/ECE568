#include "client.h"
#include <sstream>
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
    try{
        Client client("127.0.0.1",PORT);
        cout<<"Successfully connected"<<endl;
        string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"15\"><query id=\"2\"/><cancel id=\"3\"/></transactions>";
        sendString(client.socket_fd,xml);
        string ans = recvString(client.socket_fd);
        cout<<ans<<endl;
    }catch(const exception & e){
        cerr<<e.what()<<endl;
    }
    try{
        Client client("127.0.0.1",PORT);
        cout<<"Successfully connected"<<endl;
        string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"15\"><order sym=\"SYM\" amount=\"50\" limit=\"1\"/></transactions>";
        sendString(client.socket_fd,xml);
        string ans = recvString(client.socket_fd);
        cout<<ans<<endl;
    }catch(const exception & e){
        cerr<<e.what()<<endl;
    }
    for(int i = 0 ; i < 50 ; i++){
        try{
            Client client("127.0.0.1",PORT);
            cout<<"Successfully connected"<<endl;
            stringstream ss;
            ss<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?><create><account id=\""<<i + 1<<"\" balance=\""<<(i + 1 ) * 100<<"\"/><symbol sym=\"SYM\"><account id=\""<<i + 1<<"\">100</account></symbol></create>";
            sendString(client.socket_fd,ss.str());
            string ans = recvString(client.socket_fd);
            cout<<ans<<endl;
        }catch(const exception e){
            cerr<<e.what()<<endl;
        }
    }
}