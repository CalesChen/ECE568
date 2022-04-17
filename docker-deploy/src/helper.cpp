#include "helper.h"

int Helper::server_start(const char * port){

  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo* host_info_list;
  const char* hostname = NULL;

  memset(&host_info, 0, sizeof(host_info));

  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  host_info.ai_flags = AI_PASSIVE;
  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if(status != 0){
    cerr << "Error: cannot get address info for host" <<endl;
    cerr << " (" << hostname<<","<<port<<")"<<endl;
    // Original Example shows exit(FAILURE), while this is a function, so return -1
    return -1;
    //exit(EXIT_FAILURE);
  }

  socket_fd = socket(host_info_list->ai_family,
                     host_info_list->ai_socktype,
                     host_info_list->ai_protocol);
  if(socket_fd == -1){
    cerr << "Error: cannot create socket"<<endl;
    cerr << " (" << hostname << ","<<port<<")"<<endl;
    return -1;
    //exit(EXIT_FAILURE);
  }

  int yes = 1;
  status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);

  if(status == -1){
    cerr << "Error: cannot bind socket"<<endl;
    cerr << " (" <<hostname<<"," <<port <<")"<<endl;
    return -1;
    //exit(EXIT_FAILURE);
  }

  status = listen(socket_fd, 100);
  if(status == -1){
    cerr<<"Error: cannot listen on socket" <<endl;
    cerr<<" ("<<hostname<<","<<port<<")"<<endl;
    return -1;
    //exit(EXIT_FAILURE);
  }
  freeaddrinfo(host_info_list);
  return socket_fd;
}

int Helper::client_start(const char * hostname, const char * port){

  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo* host_info_list;

  memset(&host_info, 0, sizeof(host_info));
  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if(status != 0){
    cerr << "Error: cannot get address info for host_Client" <<endl;
    cerr << " (" << hostname << "," <<port << ")" <<endl;
    return -1;
    //exit(EXIT_FAILURE);
  }

  socket_fd = socket(host_info_list->ai_family,
                     host_info_list->ai_socktype,
                     host_info_list->ai_protocol);

  if(socket_fd == -1){
    cerr << "Error: cannot create socket" <<endl;
    cerr<< " (" << hostname<<","<<port<<")"<<endl;
    return -1;
    //exit(EXIT_FAILURE);
  }

  status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if(status == -1){
    cerr<<"Error:cannot connect to socket"<<endl;
    cerr<<" ("<<hostname<<","<<port<<")"<<endl;
    return -1;
    //exit(EXIT_FAILURE);
  }

  freeaddrinfo(host_info_list);
  
  return socket_fd;
}


int Helper::server_accept(int socket_fd, std::string * ip){
  struct sockaddr_storage socket_addr;
  socklen_t socket_addr_len = sizeof(socket_addr);
  int client_connection_fd;
  client_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);

  struct sockaddr_in * addr = (struct sockaddr_in *)&socket_addr;
  if(client_connection_fd == -1){
    cerr<<"Error: cannot accept connection on socket" <<endl;
    return -1;
    //exit(EXIT_FAILURE);
  }

  *ip = inet_ntoa(addr->sin_addr);
  
  return client_connection_fd;
}

//Receve message related Function. 
bool Helper::check_HTTP_tail_Chunk(vector<char> * response){
    size_t len = (*response).size();
    if((*response)[len - 5] == '0' && (*response)[len - 4] == '\r' && (*response)[len - 3] == '\n' &&
       (*response)[len - 2] == '\r' && (*response)[len - 1] == '\n'){
           return true;
       }
    return false;
}

bool Helper::check_HTTP_tail(vector<char> * response){
    size_t len = (*response).size();
    if((*response)[len - 4] == '\r' && (*response)[len - 3] == '\n' &&
       (*response)[len - 2] == '\r' && (*response)[len - 1] == '\n'){
           return true;
       }
    return false;
}


int Helper::recv_message(int socket_fd, std::vector<char> * buffer, bool isChunk){
    // May be parallel later
    // If the buffer is not initial, then resize it. 
    if(buffer->size() != 1 ) {
        buffer->resize(buffer->size() + 1);
    }
    int recv_len = 0;
    int i = buffer->size() - 1;
    while((recv_len = recv(socket_fd, &( (*buffer).data()[i] ), 1, 0) ) > 0){
        size_t len = (*buffer).size();

        if(len > 5){
            // If get to the last chunk, then close the socket
            if(isChunk){
                if( check_HTTP_tail_Chunk(buffer) ){
                    return len;
                }
            }
            else{
                if( check_HTTP_tail(buffer) ){
                    return len;
                }
            }
        }
        (*buffer).resize( len + recv_len);
        i = len;
    }
    return (*buffer).size();
}



