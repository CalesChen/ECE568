#include "client.h"

void Client::init_client(){
    memset(&this->host_info, 0, sizeof(this->host_info));

    this->host_info.ai_family   = AF_UNSPEC;
    this->host_info.ai_socktype = SOCK_STREAM;

    this->status = getaddrinfo(this->hostname, this->port, &this->host_info, &this->host_info_list);
    if (this->status != 0) {
        throw MyException("ERROR cannot get address info for host\n",this->getHostPortInfo());
    }

    this->socket_fd = socket(this->host_info_list->ai_family, 
		     this->host_info_list->ai_socktype, 
		     this->host_info_list->ai_protocol);
    if (this->socket_fd == -1) {
        throw MyException("ERROR cannot create socket\n",this->getHostPortInfo());
    }

    this->status = connect(this->socket_fd, this->host_info_list->ai_addr, this->host_info_list->ai_addrlen);
    if (this->status == -1) {
        throw MyException("ERROR cannot connect to socket\n",this->getHostPortInfo());
    }
}

string Client::getHostPortInfo(){
    string ans;
    if(this->hostname==NULL || this->port==NULL){
        cerr<<"hostname or port info is not available"<<endl;
    }else{
        ans.append("(");
        ans.append(this->hostname);
        ans.append(",");
        ans.append(this->port);
        ans.append(")");
    }
    return ans;
}

void *keepFlooding(void *accountID_ptr){
    int account_id = *((int *)accountID_ptr);
    try{
        Client client("127.0.0.1",PORT);
        cout<<"Successfully connected"<<endl;
        stringstream ss;
        ss<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?><create><account id=\""<<account_id<<"\" balance=\""<<100000000<<"\"/><symbol sym=\"BTC\"><account id=\""<<account_id<<"\">300</account></symbol></create>";
        sendString(client.socket_fd,ss.str());
        string ans = recvString(client.socket_fd);
        cout<<ans<<endl;
    }catch(const exception &e){
        cerr<<e.what()<<endl;
    }
}