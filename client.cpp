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

void Client::sendString(string msg){
    int status = send(this->socket_fd,msg.data(),msg.size()+1,0);
    if(status==-1){
        throw MyException("Error while sending ",msg);
    }
}

string Client::recvString(){
    char content[65536] = {0};
    int recv_len = recv(this->socket_fd,content,65536,0);
    if(recv_len<=0){
        throw MyException("Error while receiving","");
    }
    string ans(content,recv_len);
    return ans;
}