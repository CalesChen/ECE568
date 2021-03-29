#include "handler.h"

void *xmlHandler(void *client_fd_ptr){
    int client_fd = *((int *)client_fd_ptr);
    // cout<<client_fd<<endl;
    //recv xml from client
    string request = recvString(client_fd);

    //parse xml
    if(request.find("create")){
        Create create(request);
        
        //handlet();
    }
    else if(request.find("transactions")){
        Transaction transaction(request);
        
        //handleCreate();
    }else{
        //errorMessage.
    }
    //manipulate database

    //send feedback to client
    sendString(client_fd,"received your message: "+request);
    close(client_fd);
}