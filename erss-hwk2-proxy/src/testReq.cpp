#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
//#include "request_info.h"
#include "response.h"

int main(int argc, char ** argv){
  // std::string request("CONNECT https://www.google.com:443 HTTP/1.1\r\nHost: www.google.com:443\r\n\r\n");
  std::string response("HTTP/1.1 200 OK\r\nServer: nginx\r\nDate: Mon, 20 Feb 2017 09:13:59 GMT\r\nCache-Control:max-age=60\r\nExpires: Thu, 01 Jan 1970 00:00:00 GMT\r\n\r\n");
  // request_info req(request);
  Response resp(response);
  //req.getMethod_URI();
  // std::cout<<req.method<<std::endl;
  // std::cout<<req.host<<std::endl;
  std::cout<<resp.date<<std::endl;
  std::cout<<resp.Expires<<std::endl;
   resp.timeValid(1);
}

