#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "request_info.h"
std::ofstream logFile("/var/log/erss/proxy.log");
//必须要用portNUM吗 这是谁的portnum？？？？？？？？？
//502报错是啥玩意儿 what's bad gateway？？？？？
//如果contentlength跟实际不match 在哪check？报什么错？
void Proxy::handleProxy(){
	//create a socket to connect with client, return this socket's id
	int server_fd = server_start(portNum);
	int thread_id = 0;
	//get client id that accpeted by proxy successfully
	while(1){
		std::string server_ip;
		int client_fd = server_accept(server_fd,&server_ip);
		if(client_fd == -1){
			continue;
		}
		handleReq(server_fd,client_fd,thread_id,server_ip);
		thread_id++;		
	}
}

void Proxy::handleReq(int server_fd,int client_fd, int thread_id, std::string ip){
	//初始化？？？？？？？？？？
	 vector<char> ori_request(1, 0);
	 //################
	 //receive original message
	 int size_req = receive(client_fd,&ori_request.data()[index],1,0);
	 //convert it  to string and parse it
	 std::string request;
	 request.insert(request.begin(),ori_request.begin(),ori_request.end());
	 //需不需要delete？？？？？？？
	 request_info * parsedRequest = new request_info(request);
	 //logfile可不可以改？？？？？？？？？
	 std::string method = parsedRequest->method;
	 logFile << thread_id << ": \"" << parsedRequest->request_line << "\" from "<< ip << " @ " << getCurrTime().append("\0");
	 if(method == "GET"){
	 	//####################
	 	handleGet();
	 } else if(method == "POST"){
	 	//##############
	 	int oriServer_fd = connectOriginalServer(parsedRequest);
	 	handlePOST();
	 } else if(method == "CONNECT"){
	 	//##############
	 	int oriServer_fd = connectOriginalServer(parsedRequest);
	 	handleConnect();
	 }
}

int Proxy::connectOriginalServer(request_info * parsedRequest){
	int oriServer_fd = client_start(parsedRequest->host, parsedRequest->port);
	return oriServer_fd;
}

std::string Proxy::getCurrTime(){
	time_t  now =time(0);
	char  * data = ctime(&now)
	tm  * gm =gmtime(&now);
	data = asctime(gm);
	return std::string(data);
}

//####################################################################################################
// int Proxy::recive(int socket_fd, void * buffer, size_t size, bool isChunk){
//     if(isChunk){
//         return Proxy::recv_chunk(socket_fd, (vector<char> *)buffer);
//     }
//     return recv(socket_fd, buffer, size, 0);
// }

// Call this function when the HTTP is Chunk
int Proxy::recv_message(int socket_fd, vector<char> * buffer, bool isChunk){
    // May be parallel later
    int recv_len = 0;
    int i = 0;
    while((recv_len = recv(socket_fd, &( (*buffer)[i] ), 1, 0) ) > 0){
        size_t len = (*buffer).size();

        if(len > 5){
            // If get to the last chunk, then close the socket
            if(isChunk){
                if( check_HTTP_tail_Chunk(buffer) ){
                    close(socket_fd);
                    break;
                }
            }
            else{
                if( check_HTTP_tail(buffer) ){
                    close(socket_fd);
                    break;
                }
            }
        }
        (*buffer).resize( len + recv_len);
        i = len;
    }
}

bool check_HTTP_tail_Chunk(vector<char> * response){
    size_t len = (*response).size();
    if((*response)[len - 5] == '0' && (*response)[len - 4] == '\r' && (*response)[len - 3] == '\n' &&
       (*response)[len - 2] == '\r' && (*response)[len - 1] == '\n'){
           return true;
       }
    return false;
}

bool check_HTTP_tail(vector<char> * response){
    size_t len = (*response).size();
    if((*response)[len - 4] == '\r' && (*response)[len - 3] == '\n' &&
       (*response)[len - 2] == '\r' && (*response)[len - 1] == '\n'){
           return true;
       }
    return false;
}
