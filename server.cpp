#include "server.h"

void Server::init_serverSocket(){
    memset(&this->host_info, 0, sizeof(this->host_info));

    this->host_info.ai_family   = AF_UNSPEC;
    this->host_info.ai_socktype = SOCK_STREAM;
    this->host_info.ai_flags    = AI_PASSIVE;

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

    int yes = 1;
    this->status = setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    this->status = ::bind(this->socket_fd, this->host_info_list->ai_addr, this->host_info_list->ai_addrlen);
    if (this->status == -1) {
        throw MyException("ERROR cannot bind socket\n",this->getHostPortInfo());
    }

    this->status = listen(this->socket_fd, 100);
    if (this->status == -1) {
        throw MyException("ERROR cannot listen on socket\n",this->getHostPortInfo());
    }
}

int Server::accept_connection(string *ip_addr){
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(this->socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (client_connection_fd == -1) {
        throw MyException("ERROR cannot accept connection on socket","");
    }
    *ip_addr = inet_ntoa(((struct sockaddr_in *)&socket_addr)->sin_addr);
    return client_connection_fd;
}

string Server::getHostPortInfo(){
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