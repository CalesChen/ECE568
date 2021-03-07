#include "proxy.h"

std::ofstream logFile("/var/log/erss/proxy.log");
//必须要用portNUM吗 这是谁的portnum？？？？？？？？？
//502报错是啥玩意儿 what's bad gateway？？？？？
//如果contentlength跟实际不match 在哪check？报什么错？
void Proxy::handleProxy(Cache* cache){
	//create a socket to connect with client, return this socket's id
    Helper h;
	int server_fd = h.server_start(portNum);
	int thread_id = 0;
	//get client id that accpeted by proxy successfully
	while(1){
		std::string server_ip;
		int client_fd = h.server_accept(server_fd,&server_ip);
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
     Helper h;
	 int size_req = h.recv_message(client_fd,&ori_request,false);
	 //convert it  to string and parse it
	 std::string request;
	 request.insert(request.begin(),ori_request.begin(),ori_request.end());
	 //需不需要delete？？？？？？？
	 request_info * parsedRequest = new request_info(request);
	 //logfile可不可以改？？？？？？？？？
	 std::string method = parsedRequest->method;
	 logFile << thread_id << ": \"" << parsedRequest->request_line << "\" from "<< ip << " @ " << getCurrTime().append("\0");
     cout<<"The Method is "<<method<<endl;
	int oriServer_fd = connectOriginalServer(parsedRequest);
     if(method == "GET"){
	 	//####################
        cout<<"In GET"<<endl;
	 	handleGet(client_fd, oriServer_fd,thread_id, parsedRequest, cache);
	 } else if(method == "POST"){
	 	//##############
	 	handlePost(client_fd,oriServer_fd, thread_id, parsedRequest);
	 } else if(method == "CONNECT"){ 
	 	//##############
        cout<<method<<endl;
	 	handleConnect(client_fd, oriServer_fd, thread_id);
	 } else {
         char * msg = "HTTP/1.1 400 Bad Request";
         send(client_fd,msg,sizeof(msg),0);
         logFile << thread_id << ": Resquesting \"HTTP/1.1 400 Bad Request\"" << std::endl;
     }
     delete parsedRequest;
}

int Proxy::connectOriginalServer(request_info * parsedRequest){
    Helper h;
    //cout<<parsedRequest->request<<endl;
    //cout<<parsedRequest->host<<endl<<parsedRequest->port<<endl;
	int oriServer_fd = h.client_start((parsedRequest->host).c_str(), (parsedRequest->port).c_str());
    //int oriServer_fd = h.client_start("baidu.com", (parsedRequest->port).c_str());
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
    Response* temp;
    //Response* temp = cache->getCache(request->uri, server_fd, thread_id, request->request_line);
    //if request need to get resource from original server
    if(request->CacheControl.find("no-store")!=std::string::npos){
        temp = NULL;
    } else{
        temp = cache->getCache(request->uri, server_fd, thread_id, request);
    }

    // For no cache
    
    if(temp == NULL){
        //logFile << thread_id << ": not in cache"<<endl;
        //string request_temp(request->begin(), request->end());
        //request_info request_t(request_temp);

        logFile << thread_id << ": Requesting \"" << request->request_line << "\" from "<< request->uri << endl;
        send(server_fd, request->request.c_str(), request->request.size(), 0);
        cout<<request->request<<endl<<request->request.size()<<endl;
        // THis function will receive the message and send it to the client. 
        // I need more information for put the message into Cache
        
        Proxy::ServerGet(client_fd, server_fd, thread_id, request, cache);
    }
    // If I can find a match in the temp, I will just send the response?
    else{
        cout << temp->firstLine<<endl;
        send(client_fd, temp->response.c_str(), temp->response.size(), 0);
    }

}
void Proxy::ServerGet(int client_fd, int server_fd, int thread_id, request_info * request, Cache * cache){
    vector<char> server_msg(1,0);
    // Receive the First part
    Helper h;
    int server_msg_len = h.recv_message(server_fd, &server_msg, false);
    cout<<server_msg_len<<endl;
    // for(int i = 0 ; i < server_msg_len ; i ++){
    //     cout<<server_msg[i];
    // }
    cout<<endl;
    //No response
    if(server_msg_len == 0) return;
    // Store the message to a string
    string first_part(server_msg.begin(), server_msg.end());
    cout<<"String"<<first_part;
    Response resp(first_part);
    //resp.parseResponse();
    cout<<"Get_Thread_Id" << thread_id<<endl;
    logFile << thread_id << ": Received \"" << resp.firstLine<< " \" from " << request->uri<<endl;

    bool isChunk = false;
    int pos;
    if((pos = first_part.find("chunked"))  != string::npos){
        isChunk = true;
    }
    cout<<isChunk;
    // Check if we need to add it into Cache
    bool no_store = false;
    if((pos = first_part.find("no-store")) != string::npos){
        no_store = true;
    }
    cout<<no_store;
    // If it is not Chunk, the first recv_message func will give back the right answer. 
    if(isChunk){
        server_msg_len = h.recv_message(server_fd, &server_msg, isChunk);
    }else{
        server_msg_len = h.recv_message(server_fd, &server_msg, isChunk);
    }

    cout<<server_msg.size()<<endl;
    string all(server_msg.begin(), server_msg.end());
    cout<<all;
    cout<<send(client_fd, all.c_str(), all.size(), 0)<<endl;
    

    // Placeholder for adding to the cache

    
    Response resp_all(all);

    cache->putCache(resp_all, request->uri, thread_id);

    logFile<<thread_id<<": Responding \"" <<resp.firstLine << "\""<<endl;


}

void Proxy::handlePost(int client_fd, int server_fd, int thread_id, request_info * request){
    
    // The last parameter will help to avoid "send func" send exception. 
    // We will handle it by the response of the server. 
    send(server_fd, request->request.c_str(), request->request.size(), MSG_NOSIGNAL);
    vector<char> response(1,0);
    Helper h;
    int response_len = h.recv_message(server_fd, &response, false);

    //string response_str(response.begin(), response.end());
    if(response_len != 0){
        // Which parameter trans in.
        // response res;
        // res.parseResponse();
        cout<<response_len<<endl;
        std::string temp(response.begin(), response.begin() + response_len);
        cout<<temp<<endl;
        Response res(temp); 
        res.parseResponse();

        // How to get the first line in the response?
        logFile << thread_id << ": Received \"" << res.firstLine << "\" from " << request->uri << endl;
        send(client_fd, temp.c_str(), response_len, MSG_NOSIGNAL);

        logFile << thread_id << ": Responding \""<<res.firstLine<<endl;

    }
    else{
        cout<<"Server Socket Closed\n";
    }
}

void Proxy::handleConnect(int client_fd, int server_fd, int thread_id){
    string msg = "HTTP/1.1 200OK\r\n\r\n";
    send(client_fd, msg.c_str() , msg.size(), 0);

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
        int i = 0;
        for(int i = 0 ; i < 2 ; i ++){
            //vector<char> msg2(65536,0);
            char msg2[65536] = {0};
            if(FD_ISSET(fd_set_cs[i], &fds)){
                //len_recv = Proxy::recv_message(fd_set_cs[i], &msg2, false);
                len_recv = recv(fd_set_cs[i], msg2, sizeof(msg2), 0);
                if(len_recv <= 0){
                    return;
                }
                //string temp(msg2.begin(), msg2.end());
                // Check the len_send and len_recv
                //len_send = send(fd_set_cs[1-i], temp.c_str(), len_recv, 0);
                len_send = send(fd_set_cs[1-i], msg2, len_recv, 0);
                // If all of the message is done.
                if(len_send <=0){
                    return;
                }
            }
            i++;
        } 
    }
}


// void Proxy::handleConnect(int client_fd, int server_fd, int id) {
//   send(client_fd, "HTTP/1.1 200 OK\r\n\r\n", 19, 0);
//   logFile << id << ": Responding \"HTTP/1.1 200 OK\"" << std::endl;

//   fd_set readfds;
//   int nfds = server_fd > client_fd ? server_fd + 1 : client_fd + 1;

//   while (1) {
//     FD_ZERO(&readfds);
//     FD_SET(server_fd, &readfds);
//     FD_SET(client_fd, &readfds);

//     select(nfds, &readfds, NULL, NULL, NULL);
//     int fd[2] = {server_fd, client_fd};
//     int len;
//     for (int i = 0; i < 2; i++) {
//       char message[65536] = {0};
//       if (FD_ISSET(fd[i], &readfds)) {
//         len = recv(fd[i], message, sizeof(message), 0);
//         if (len <= 0) {
//           return;
//         }
//         else {
//           if (send(fd[1 - i], message, len, 0) <= 0) {
//             return;
//           }
//         }
//       }
//     }
//   }
// }
