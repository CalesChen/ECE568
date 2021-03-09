#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <vector>

using namespace std;
class Helper{
    public:
        bool check_HTTP_tail_Chunk(vector<char> * response);
        bool check_HTTP_tail(vector<char> * response);
        int recv_message(int socket_fd, vector<char> * buffer, bool isChunk);
        int server_accept(int socket_fd, std::string * ip);
        int client_start(const char * hostname, const char * port);
        int server_start(const char * port);
};

