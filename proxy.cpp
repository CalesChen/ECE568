#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <fstream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "request_info.h"
#include "proxy.h"
#include "response.h"

std::ofstream logFile("/var/log/erss/proxy.log");
//必须要用portNUM吗 这是谁的portnum？？？？？？？？？
//502报错是啥玩意儿 what's bad gateway？？？？？
//如果contentlength跟实际不match 在哪check？报什么错？
void Proxy::handleProxy(Cache* cache){
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
		handleReq(server_fd,client_fd,thread_id,server_ip, cache);
		thread_id++;		
	}
}

void Proxy::handleReq(int server_fd,int client_fd, int thread_id, std::string ip,Cache* cache){
	 std::vector<char> ori_request(1, 0);
	 //receive original message
	 int index = 0;
	 int size_req = recv_message(client_fd,&ori_request,false);
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
        int oriServer_fd = connectOriginalServer(parsedRequest);
	 	Proxy::handleGet(client_fd, oriServer_fd, thread_id, parsedRequest, cache);
	 } else if(method == "POST"){
	 	//##############
	 	int oriServer_fd = connectOriginalServer(parsedRequest);
	 	Proxy::handlePost(client_fd,oriServer_fd, thread_id, parsedRequest);
	 } else if(method == "CONNECT"){ 
	 	//##############
	 	int oriServer_fd = connectOriginalServer(parsedRequest);
	 	Proxy::handleConnect(client_fd, oriServer_fd, thread_id);
	 }

}

int Proxy::connectOriginalServer(request_info * parsedRequest){
	int oriServer_fd = client_start((parsedRequest->host).c_str(), (parsedRequest->port).c_str());
	return oriServer_fd;
}

std::string Proxy::getCurrTime(){
	time_t  now =time(0);
	const char * data = ctime(&now);
	struct tm * gm =gmtime(&now);
	data = asctime(gm);
	return data;
}

//**********Ke Chen's Code Start****************//

void Proxy::handleGet(int client_fd, int server_fd, int thread_id, request_info * request, Cache* cache){
    Response* temp = cache->getCache(request->uri, server_fd, thread_id, request->request_line);
    // For no cache
    if(temp == NULL){
        //logFile << thread_id << ": not in cache"<<endl;
        //string request_temp(request->begin(), request->end());
        //request_info request_t(request_temp);

        logFile << thread_id << ": Requesting \"" << request->request_line << "\" from "<< request->uri << endl;
        send(server_fd, request, request->request.size(), 0);

        // THis function will receive the message and send it to the client. 
        // I need more information for put the message into Cache
        
        Proxy::ServerGet(client_fd, server_fd, thread_id, request, cache);
    }
    // If I can find a match in the temp, I will just send the response?
    else{
        send(client_fd, &(temp->response), temp->response.size(), 0);
    }

}
void Proxy::ServerGet(int client_fd, int server_fd, int thread_id, request_info * request, Cache * cache){
    vector<char> server_msg(1,0);
    // Receive the First part
    int server_msg_len = recv_message(server_fd, &server_msg, false);

    //No response
    if(server_msg_len == 0) return;
    // Store the message to a string
    string first_part(server_msg.begin(), server_msg.end());
    Response resp(first_part);
    //resp.parseResponse();

    logFile << thread_id << ": Received \"" << resp.firstLine<< " \" from " << request->uri<<endl;

    bool isChunk = false;
    int pos;
    if((pos = first_part.find("chunked"))  != string::npos){
        isChunk = true;
    }
    
    // Check if we need to add it into Cache
    bool no_store = false;
    if((pos = first_part.find("no-store")) != string::npos){
        no_store = true;
    }

    // If it is not Chunk, the first recv_message func will give back the right answer. 
    if(isChunk){
        server_msg_len = recv_message(server_fd, &server_msg, isChunk);
    }

    send(client_fd, &server_msg, server_msg.size(), 0);

    // Placeholder for adding to the cache

    string all(server_msg.begin(), server_msg.end());
    Response resp_all(all);

    cache->putCache(resp_all, request->uri, thread_id);

    logFile<<thread_id<<": Responding \"" <<resp.firstLine << "\""<<endl;


}

void Proxy::handlePost(int client_fd, int server_fd, int thread_id, request_info * request){
    
    // The last parameter will help to avoid "send func" send exception. 
    // We will handle it by the response of the server. 
    send(server_fd, &request->request, request->request.size(), MSG_NOSIGNAL);
    vector<char> response(1,0);
    
    int response_len = recv_message(server_fd, &response, false);

    if(response_len != 0){
        // Which parameter trans in.
        // response res;
        // res.parseResponse();
        std::string temp(response.begin(), response.begin() + response_len);
        
        Response res(temp); 
        res.parseResponse();

        // How to get the first line in the response?
        logFile << thread_id << ": Received \"" << res.firstLine << "\" from " << request->uri << endl;
        send(client_fd, &response, response_len, MSG_NOSIGNAL);

        logFile << thread_id << ": Responding \""<<res.firstLine<<endl;

    }
    else{
        cout<<"Server Socket Closed\n";
    }
}

void Proxy::handleConnect(int client_fd, int server_fd, int thread_id){
    string msg = "HTTP/1.1 200OK\r\n\r\n";
    send(client_fd, &msg , msg.size(), 0);

    logFile << thread_id << ": Responding \"HTTP/1.1 200 OK\""<<endl;

    fd_set fds;
    int nfds = max(client_fd, server_fd) ;

    bool flag = true;
    while(flag){
        FD_ZERO(&fds);
        FD_SET(server_fd, &fds);
        FD_SET(client_fd, &fds);
        select(nfds + 1,&fds, NULL, NULL, NULL);
        vector<int> fd_set_cs{client_fd, server_fd};

        int len_recv;
        int len_send;
        for(int i = 0 ; i < 2 ; i ++){
            vector<char> msg2(65536,0);
            if(FD_ISSET(fd_set_cs[i], &fds)){
                //len_recv = Proxy::recv_message(fd_set_cs[i], &msg2, false);
                len_recv = recv(fd_set_cs[i], &msg2, sizeof(msg2), 0);
                if(len_recv <= 0){
                    return;
                }
                // Check the len_send and len_recv
                len_send = send(fd_set_cs[1-i], &msg2, len_recv, 0);
                // If all of the message is done.
                if(len_send <=0){
                    return;
                }
            }

        } 
    }
}
