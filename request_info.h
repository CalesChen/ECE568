#include <iostream>
#include <string>

class request_info{
  public:
  	std::string request;
    std::string host;
    std::string method;
    std::string uri;
    std::string request_line;
    std::string header;
    std::string body;
    std::string content_len;

    request_info(std::string request):request(request){
    	parseRequest();
    }
	void getHeader_body_contentLen();
	void getRequestLine();
    void getMethod_URI();
    void getHost();
    void parseRequest();
  
}