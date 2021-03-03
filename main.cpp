#include "proxy.h"

int main(int argc, char ** argv){

    int capacity = atoi(argv[1]);
    Cache proxyCache(capacity);
    Proxy myProxy(argv[2]);

    myProxy.handleProxy(&proxyCache);
    
}
