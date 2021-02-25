int status;
struct addrinfo hints;
struct addrinfo *servinfo;  // will point to the results

int create_server(){
  int status;
  int sockfd;
  struct addrinfo hints;
  struct addrinfo *servinfo;  // will point to the results
  const char hostname;
  
  memset(&hints, 0, sizeof hints); // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

  if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)

// bind it to the port we passed in to getaddrinfo():
int yes=1;
if (setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
    perror("setsockopt");
    exit(1);
} 
  bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
  freeaddrinfo(servinfo); // free the linked-list
}