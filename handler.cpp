#include "handler.h"

void *xmlHandler(void *client_fd_ptr){
    int client_fd = *((int *)client_fd_ptr);
    // cout<<client_fd<<endl;
    //recv xml from client
    connection *C;
    connectDB(DB_INFO,&C);

    string request = recvString(client_fd);

    stringstream ss;
    ErrorMSG err;
    Result res;
    ss<<"<results>";
    //parse xml
    if(request.find("create")){
        Create create(request);
        
        //handleCreate
        //create.account_balance.size() == create.account_id.size 
        // Create Acount
        for(int i = 0 ; i < create.account_id.size();i++){
            // Create Account
            if(create.account_id[i] == 0){
                string temp = "Sorry, the account ID is invalid.";
                ss<<err.createAccountErrorMSG(create.origin_id[i], temp);
            }else if(createAccount(C,create.account_id[i], create.account_balance[i])){
                // Add Success
                ss<<res.createAccountResult(create.account_id[i]);
            }else{
                // Add Failure
                string temp = "Account already exists";
                ss<<err.createAccountErrorMSG(create.account_id[i], temp);
            }
        }
        for(int i = 0 ; i < create.symbol.size() ; i ++){
            if(create.symbol[i].account_id.size() < 1){
                string temp = "The symbol node is empty.";
                // If there is no child node, then there will be no account Id. So I use 0.
                ss<<err.createSymbolErrorMSG(0, create.symbol[i].symbol, temp);
            }else if(!checkName(create.symbol[i].symbol)){
                string temp = "The symbold name is invalid.";
                ss<<err.createSymbolErrorMSG(0, create.symbol[i].symbol, temp);
            }else{
                for(int j = 0; j < create.symbol[i].account_id.size(); i++){
                    if(create.symbol[i].account_id[j] == 0){
                        string temp = "The Account is invalid";
                        ss<<err.createSymbolErrorMSG(create.symbol[i].orgin_id[j],create.symbol[i].symbol, temp);
                    }
                    else if(accountExist(C,create.symbol[i].account_id[j])){
                        string temp = "The Account does not exist";
                        ss<<err.createSymbolErrorMSG(create.symbol[i].symbol, create.symbol[i].symbol, temp);
                    }else{
                        ss<<res.createSymbolResult(create.symbol[i].symbol, create.symbol[i].account_id[j]);
                    }
                }
            }
        }
        
    }
    else if(request.find("transactions")){
        Transaction transaction(request);
        //handleTransaction
        // Check if the account_id is valid
        
        if(transaction.account_id == 0){
            int amount = transaction.cancels.size() + transaction.orders.size() + transaction.querys.size();
            string temp = "Sorry, the account ID is invalid.";
            for(int i = 0 ; i < amount ; i++){
                ss<<err.createAccountErrorMSG(transaction.origin, temp);
            }            
        } else if(accountExist(C,transaction.account_id)){     // Account_id not exists waiting for function!
            int amount = transaction.cancels.size() + transaction.orders.size() + transaction.querys.size();
            string temp = "Sorry, the account doesn't exist";
            for(int i = 0 ; i < amount ; i++){
                ss<<err.transactionEmptyErrorMSG(transaction.account_id, temp);
            }
        }else if(transaction.cancels.size() + transaction.orders.size() + transaction.querys.size() == 0){
            string temp = "Sorry, the requested XML doesn't have any content.";
            ss<<err.transactionEmptyErrorMSG(transaction.account_id, temp);
        }else{
            // Handle Order
            for(int i = 0 ; i < transaction.orders.size();i++){
                if(!checkName(transaction.orders[i].sym)){
                    string temp = "The symbold name is invalid.";
                    ss<<err.createSymbolErrorMSG(0, transaction.orders[i].sym,temp);
                }else{
                    // General Process
                }
            }
            // Handle Query
            for(int i = 0 ; i < transaction.querys.size() ; i++){
                if(transactionExist(C,transaction.querys[i])){  // transaction ID does not exist
                    string temp = "Sorry, the transaction ID does not exist";
                    ss<<err.queryErrorMSG(transaction.querys[i], temp);
                }else{
                    // Query Result.
                }
            }
            // Handle Cancel

            for(int i = 0 ; i < transaction.cancels.size() ; i++){
                // if(transaction.cancels[i] == 0){
                //     string temp = "Sorry, the transaction ID is not valid";
                //     ss<<"<error>"<<temp<<"</error>";
                // }else 
                if(transactionExist(C,transaction.cancels[i])){  // transaction ID does not exist
                    string temp = "Sorry, the transaction ID does not exist";
                    ss<<err.cancelErrorMSG(transaction.querys[i], temp);
                }else{
                    // cancel Result.
                }
            }
        }
        
    }else{
        //errorMessage.
        ss<<"<error>Unable to handle this request<error/>";
    }
    //manipulate database
    disconnectDB(C);
    //send feedback to client, send ret.
    string ret = ss.str();
    sendString(client_fd,"received your message: "+request);
    close(client_fd);
}

bool checkName(string & sym){
    for(int i = 0 ; i < sym.size() ; i ++){
        if(isalnum(sym[i]) == 0){
            return false;
        }
    }
    return true;
}