#include "proxy.h"

int main(int argc, char ** argv){

    //int capacity = atoi(argv[1]);
    //Cache proxyCache(capacity);
    const char * port = "12345";
    Proxy myProxy(port);

    myProxy.handleProxy(50);
    
}
