#include "client.h"

int main(){
    try{
        Client client("127.0.0.1",PORT);
        cout<<"Successfully connected"<<endl;
        string greeting = client.recvString();
        cout<<greeting<<endl;
        client.sendString("fjklds");
    }catch(const exception e){
        cerr<<e.what()<<endl;
    }
}