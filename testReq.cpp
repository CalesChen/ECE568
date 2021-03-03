#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include "request_info.h"

int main(int argc, char ** argv){

  request_info req("GET / HTTP/1.1\r\nHost: www.mof.g.cn\r\n\r\n");
  std::cout<<req.method<<std::endl;
  std::cout<<req.host<<std::endl;
}

