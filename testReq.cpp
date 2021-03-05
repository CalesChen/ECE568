#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include "request_info.h"
#include "response.h"

int main(int argc, char ** argv){
  std::string request("CONNECT https://www.google.com:443 HTTP/1.1\r\nHost: www.google.com:443\r\n\r\n");
  request_info req(request);
  //req.getMethod_URI();
  std::cout<<req.method<<std::endl;
  std::cout<<req.host<<std::endl;
  std::cout<<req.request_line<<std::endl;
}

