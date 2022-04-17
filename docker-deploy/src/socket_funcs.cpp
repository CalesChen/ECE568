#include "socket_funcs.h"

void sendString(int fd,string msg){
    int status = send(fd,msg.data(),msg.size()+1,0);
    if(status==-1){
        throw MyException("Error while sending ",msg);
    }
}

string recvString(int fd){
    char content[65536] = {0};
    int recv_len = recv(fd,content,65536,0);
    if(recv_len<=0){
        throw MyException("Error while receiving","");
    }
    string ans(content,recv_len);
    return ans;
}

string recvXML(int fd){
    string wholeMsg = recvString(fd);
    string::size_type pos = wholeMsg.find_first_of('\n');
    if(pos==wholeMsg.npos){
        return wholeMsg;
    }
    int len = stoi(wholeMsg.substr(0,pos));
    return wholeMsg.substr(pos+1,len);
}