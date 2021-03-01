#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>

class Response{
  public:
  	std::string response;
    std::string status;
    std::string date;
    std::string ContentType;
    std::string Expires;
    int maxAge;
    std::string TransferEncoding;
    std::string header;
    std::string body;
    int content_len;
    std::string CacheControl;
    std::string Etag;
    std::string lastModified;
    std::string firstLine;
    time_t convertedDate;
    time_t convertedExpires;

    Response(std::string response):response(response){
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
     void getEtag();
    void getLastModified();
    void parseResponse();
   
  
};