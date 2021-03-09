#include "cache.h"
//logfile and pthread 都没加！！！！！！
/**
get cache corresponding to url. "Return null" means cache isn't usable and proxy need to connect original server,
if not null, proxy can respond to client using this response returned.
**/
Response* Cache::getCache(std::string url, int oriServer_fd, int thread_id, request_info * request){

	std::unordered_map<std::string, std::list<std::pair<std::string, Response> >::iterator>::iterator it = cacheMap.find(url);
	if(it != cacheMap.end()){
		Response * findedResponse = &(it->second->second);
		//check if need revalidate
		if(findedResponse->CacheControl.find("must-revalidate")!=std::string::npos ||
			findedResponse->CacheControl.find("no-cache")!=std::string::npos||
			request->CacheControl.find("no-cache")!=std::string::npos){
			//###############
			if(!revalidate(findedResponse,oriServer_fd,request->request)){
				std::ofstream file;
   				file.open("proxy.log", std::ios_base::app | std::ios_base::out);
    			file << thread_id << " : "<< "in cache, requires validation"<< std::endl;
    			file.close();
				return NULL;
			}
		}
		//check if the time expired or maxAge+date <current time, if expired, erase this response from cache, return null
		if(!findedResponse->timeValid(thread_id)){
			cacheList.erase(it->second);
			cacheMap.erase(it);
			return NULL;
		}
		//put this pair to top
		cacheList.splice(cacheList.begin(), cacheList, it->second);
		std::ofstream file;
   		file.open("proxy.log", std::ios_base::app | std::ios_base::out);
    	file << thread_id << " : "<< "in cache, valid"<< std::endl;
		file << thread_id << findedResponse->header<< std::endl;
    	file.close();
		std::cout<<"cache USED!!!!!!!!!! "<< url <<std::endl;
		std::cout << findedResponse->header<<std::endl;
		return findedResponse;
	}
	std::ofstream file;
	file.open("proxy.log", std::ios_base::app | std::ios_base::out);
	file << thread_id << " : "<< "not in cache"<< std::endl;
	file.close();
	return NULL;
}

//if response finded in cache can be used, return true, if response need to be updated return false
bool Cache::revalidate(Response * response,int oriServer_fd, std::string request){
	Helper h;
	std::string Etag = response->Etag;
	std::string lastModified = response->LastModified;
	if(Etag == "" && lastModified == ""){
		return false;
	} 
	if(Etag != ""){
		std::string modifiledEtag = "If-None-Match: "+Etag+"\r\n";
		request = request.insert(request.length()-2,modifiledEtag);
	} 
	if (lastModified!=""){
		std::string modifiledLastModified = "If-Modified-Since: "+ lastModified+"\r\n";
		request = request.insert(request.length()-2,modifiledLastModified);
	}
	const char * sendMsg = request.c_str();
	int size_send = send(oriServer_fd,sendMsg,request.length()+1,0);
	std::vector<char> ori_response(1, 0);
	 //receive original message
	int index = 0;
	int size_resp = h.recv_message(oriServer_fd,&ori_response,false);
	 //convert it  to string and parse it
	std::string newResponse;
	newResponse.insert(newResponse.begin(),ori_response.begin(),ori_response.end());
	if(newResponse.find("HTTP/1.1 200 OK")!=std::string::npos){
		//cache need to be updated
	 	return false;
	 } else {
		//cache can be used
	 	return true;
	 }
}

void Cache::putCache(Response response, std::string url, int thread_id){
	if(response.CacheControl.find("no-store")!=std::string::npos){
		return;
	}
	std::unordered_map<std::string, std::list<std::pair<std::string, Response> >::iterator>::iterator it = cacheMap.find(url);
	if (it != cacheMap.end()) {
		cacheList.erase(it->second);
	}
	cacheList.push_front(make_pair(url, response));
	cacheMap[url] = cacheList.begin();
	if(cacheMap.size()>capacity){
		std::string k = cacheList.rbegin()->first;
        cacheList.pop_back();
        cacheMap.erase(k);
	}
}

