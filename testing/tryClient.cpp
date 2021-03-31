#include "client.h"
#include <sstream>
#include <ctime>
#include <chrono>
#include <ratio>

#include<pthread.h>

#define FLOODING true

int main(){
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    // // create account 123 with balance 100000000 + create symbol BTC for account 123 with 300 shares
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         stringstream ss;
    //         ss<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?><create><account id=\""<<123<<"\" balance=\""<<100000000<<"\"/><symbol sym=\"BTC\"><account id=\""<<123<<"\">300</account></symbol></create>";
    //         sendString(client.socket_fd,ss.str());
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception e){
    //         cerr<<e.what()<<endl;
    //     }
    // // create account 124 with balance 100000000 + Create symbol BTC for account 124 with 800 shares
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         stringstream ss;
    //         ss<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?><create><account id=\""<<124<<"\" balance=\""<<100000000<<"\"/><symbol sym=\"BTC\"><account id=\""<<124<<"\">800</account></symbol></create>";
    //         sendString(client.socket_fd,ss.str());
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception e){
    //         cerr<<e.what()<<endl;
    //     }
    // //Account 123: order BTC, buy 100 at 125
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string myxml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"123\"><order sym=\"BTC\" amount=\"300\" limit=\"125\"/></transactions>";
    //         sendString(client.socket_fd,myxml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception e){
    //         cerr<<e.what()<<endl;
    //     }
    // //Account 124: order BTC, sell 100 at 130
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string myxml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"124\"><order sym=\"BTC\" amount=\"-100\" limit=\"130\"/></transactions>";
    //         sendString(client.socket_fd,myxml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception e){
    //         cerr<<e.what()<<endl;
    //     }
    // // 123: buy 200 at 127
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string myxml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"123\"><order sym=\"BTC\" amount=\"200\" limit=\"127\"/></transactions>";
    //         sendString(client.socket_fd,myxml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception e){
    //         cerr<<e.what()<<endl;
    //     }

    // // 124: sell 500 at 128
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string myxml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"124\"><order sym=\"BTC\" amount=\"-500\" limit=\"128\"/></transactions>";
    //         sendString(client.socket_fd,myxml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception e){
    //         cerr<<e.what()<<endl;
    //     }

    // // 124: sell 200 at 140
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string myxml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"124\"><order sym=\"BTC\" amount=\"-200\" limit=\"140\"/></transactions>";
    //         sendString(client.socket_fd,myxml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception e){
    //         cerr<<e.what()<<endl;
    //     }
    
    // // 123 : buy 400 at 125
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string myxml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"123\"><order sym=\"BTC\" amount=\"400\" limit=\"125\"/></transactions>";
    //         sendString(client.socket_fd,myxml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception e){
    //         cerr<<e.what()<<endl;
    //     }

    // //Create symbol BTC for account 124 with 500 shares
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         stringstream ss;
    //         ss<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?><create><symbol sym=\"BTC\"><account id=\""<<124<<"\">500</account></symbol></create>";
    //         sendString(client.socket_fd,ss.str());
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception e){
    //         cerr<<e.what()<<endl;
    //     }
    // // 124: sell 400 at 127
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string myxml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"124\"><order sym=\"BTC\" amount=\"-400\" limit=\"124\"/></transactions>";
    //         sendString(client.socket_fd,myxml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception e){
    //         cerr<<e.what()<<endl;
    //     }
    
    // // query transid 7 (res: fully exec 400, 200 at 127, 200 at 125)
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"124\"><query id=\"7\"/></transactions>";
    //         sendString(client.socket_fd,xml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception & e){
    //         cerr<<e.what()<<endl;
    //     }

    // //query transid 3 (res: fully exec 200 at 127)
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"124\"><query id=\"3\"/></transactions>";
    //         sendString(client.socket_fd,xml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception & e){
    //         cerr<<e.what()<<endl;
    //     }
    // // query transid  1 (res: partial exec, open 100, exec 200 at 125)
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"124\"><query id=\"1\"/></transactions>";
    //         sendString(client.socket_fd,xml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception & e){
    //         cerr<<e.what()<<endl;
    //     }

    // // query transid  1 (res: partial exec, open 100, exec 200 at 125)
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"124\"><cancel id=\"1\"/></transactions>";
    //         sendString(client.socket_fd,xml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception & e){
    //         cerr<<e.what()<<endl;
    //     }

    // // query transid  1 (res: partial exec, open 100, exec 200 at 125)
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"124\"><query id=\"1\"/></transactions>";
    //         sendString(client.socket_fd,xml);
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception & e){
    //         cerr<<e.what()<<endl;
    //     }
 
    // try{
    //     Client client("127.0.0.1",PORT);
    //     cout<<"Successfully connected"<<endl;
    //     string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"15\"><query id=\"2\"/><cancel id=\"3\"/></transactions>";
    //     sendString(client.socket_fd,xml);
    //     string ans = recvString(client.socket_fd);
    //     cout<<ans<<endl;
    // }catch(const exception & e){
    //     cerr<<e.what()<<endl;
    // }
    // try{
    //     Client client("127.0.0.1",PORT);
    //     cout<<"Successfully connected"<<endl;
    //     string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><transactions id=\"15\"><order sym=\"SYM\" amount=\"50\" limit=\"1\"/></transactions>";
    //     sendString(client.socket_fd,xml);
    //     string ans = recvString(client.socket_fd);
    //     cout<<ans<<endl;
    // }catch(const exception & e){
    //     cerr<<e.what()<<endl;
    // }
    // for(int i = 0 ; i < 50 ; i++){
    //     try{
    //         Client client("127.0.0.1",PORT);
    //         cout<<"Successfully connected"<<endl;
    //         stringstream ss;
    //         ss<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?><create><account id=\""<<i + 1<<"\" balance=\""<<(i + 1 ) * 100<<"\"/><symbol sym=\"SYM\"><account id=\""<<i + 1<<"\">100</account></symbol></create>";
    //         sendString(client.socket_fd,ss.str());
    //         string ans = recvString(client.socket_fd);
    //         cout<<ans<<endl;
    //     }catch(const exception e){
    //         cerr<<e.what()<<endl;
    //     }
    // }

    // long accountIDGenerator = 204;
    // while(accountIDGenerator>=200){
    //     long curr_account_id = accountIDGenerator;
    //     pthread_t thread;
    //     pthread_create(&thread,NULL,keepFlooding,&curr_account_id);
    //     accountIDGenerator--;
    // }
    if(FLOODING){
        long accountIDGenerator = 9999;
        while(accountIDGenerator>200){
            try{
                Client client("127.0.0.1",PORT);
                cout<<"Successfully connected"<<endl;
                stringstream ss;
                ss<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?><create><account id=\""<<accountIDGenerator<<"\" balance=\""<<100<<"\"/><symbol sym=\"SYM\"><account id=\""<<accountIDGenerator<<"\">100</account></symbol></create>";
                sendString(client.socket_fd,ss.str());
                string ans = recvString(client.socket_fd);
                cout<<ans<<endl;
            }catch(const exception e){
                cerr<<e.what()<<endl;
                break;
            }
            accountIDGenerator--;
        }
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    cout<<"The whole process took "<<time_span.count()<<"seconds."<<endl;
}