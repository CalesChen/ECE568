#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>

class response{
  public:
  	std::string response;
    std::string status;
    std::string date;
    std::string Content-Type;
    std::string Expires;
    std::string Transfer-Encoding;
    std::string header;
    std::string body;
    std::string content_len;
    std::string CacheControl;
    time_t convertedDate;
    time_t convertedExpires;

    request_info(std::string response):response(response){
    	parseResponse();
    }
	void getHeader_body();
	void getStatus();
	void getTransferEncoding();
    void getcontent_len();
    time_t getDate_convert();
    time_t getExpires_convert();
    void getCacheControl();
    bool timeValid();
    void parseResponse();
  
}