#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "response.h"


void Response::getHeader_body(){
	size_t pos_header_end = response.find("\r\n\r\n");
	header = response.substr(0,pos_header_end);
	body = response.substr(pos_header_end+4);
}

void Response::getFirstLine(){
	size_t pos_FirstLine = response.find("\r\n");
	firstLine = response.substr(0,pos_FirstLine);
}

void Response::getStatus(){
	size_t pos_status = header.find(" ");
	std::string mid_status = header.substr(pos_status);
	size_t pos_status_end = mid_status.find("\r\n");
	status = mid_status.substr(0,pos_status_end);

}

void Response::getTransferEncoding(){
	size_t pos_TransferEncoding = response.find("Transfer-Encoding:");
	if(pos_TransferEncoding!=std::string::npos){
		std::string mid_TransferEncoding = response.substr(pos_TransferEncoding);
		size_t pos_TransferEncoding_end = mid_TransferEncoding.find("\r\n");
		TransferEncoding = mid_TransferEncoding.substr(0,pos_TransferEncoding_end);
	}
}

void Response::getcontent_len(){
	size_t pos_content_len = response.find("Content-Length:");
	if(pos_content_len!=std::string::npos){
		std::string mid_content_len = response.substr(pos_content_len);
		size_t pos_content_len_end = mid_content_len.find("\r\n");
		std::string content_len_str = mid_content_len.substr(0,pos_content_len_end);
		content_len = atoi(content_len_str.c_str());
	}
}

void Response::getCacheControl_MaxAge(){
	size_t pos_CacheControl = response.find("Cache-Control:");
	if(pos_CacheControl!=std::string::npos){
		std::string mid_CacheControl = response.substr(pos_CacheControl);
		size_t pos_CacheControl_end = mid_CacheControl.find("\r\n");
		CacheControl = mid_CacheControl.substr(0,pos_CacheControl_end);
	}
	if(CacheControl.find("max-age")!=std::string::npos){
		size_t pos_maxAge = CacheControl.find("=");
		maxAge = atoi((CacheControl.substr(pos_maxAge+2)).c_str());
	}
}


time_t Response::getDate_convert(){
	size_t pos_date = response.find("Date:");
	if(pos_date!=std::string::npos){
		std::string mid_date = response.substr(pos_date+6);
		size_t pos_date_end = mid_date.find(" GMT");
		date = mid_date.substr(0,pos_date_end);
	}
	tm tm_;
	strptime(date.c_str(), "%a, %d %b %Y %H:%M:%S", &tm_);
	convertedDate  = mktime(&tm_);
	return convertedDate;
}

time_t Response::getExpires_convert(){
	size_t pos_expires = response.find("Expires:");
	if(pos_expires!=std::string::npos){
		std::string mid_expires = response.substr(pos_expires+9);
		size_t pos_expires_end = mid_expires.find(" GMT");
		Expires = mid_expires.substr(0,pos_expires_end);
		
	}

	struct tm tm_;
	//strptime(Expires.c_str(), "%Y-%m-%d %H:%M:%S", &tm_);
	strptime(Expires.c_str(), "%a, %d %b %Y %H:%M:%S", &tm_);
	convertedExpires  = mktime(&tm_);
	return convertedExpires;
}


bool Response::timeValid(int thread_id){
	time_t now =time(0);
	//if(maxAge>0){
		
		//if(maxAge + convertedDate <= now){
			//有别的方法进行转换嘛？？？？？？？
		    time_t exipTime = maxAge + convertedDate;
  			struct tm * tm = gmtime(&exipTime);
			struct tm * tm1 = gmtime(&convertedDate);
  			const char * time = asctime(tm);
			// std::ofstream file;
   			// file.open("proxy.log", std::ios_base::app | std::ios_base::out);
    		// //file << thread_id << " : "<< "in cache, but expired at "<< std::string(asctime(tm)) <<std::endl;
    		// file << thread_id << " : "<< "in cache, but expired at "<<time <<std::endl;
			// file << thread_id << " : "<< "in cache, but expired at date "<< asctime(tm1) <<std::endl;
			// file << thread_id << " : "<< "in cache, but expired at string Expires "<<Expires <<std::endl;
			// file << thread_id << " : "<< "in cache, but expired at string date "<< date <<std::endl;
			// //std::cout << thread_id << " : "<< "in cache, but expired at "<< asctime(tm) <<std::endl;
			// file.close();
			std::cout <<"time "<< time<<std::endl;
			std::cout <<"asctime(tm1) "<< asctime(tm1)<<std::endl;
			return false;
		//} 
	//}
	if(now>convertedExpires){
		struct tm * tm = gmtime(&convertedExpires);
		std::ofstream file;
   		file.open("proxy.log", std::ios_base::app | std::ios_base::out);
    	file << thread_id << " : "<< "in cache, but expired at EXPIRES "<< asctime(tm) <<std::endl;
    	file.close();
		std::cout <<"asctime(tm1)"<< asctime(tm)<<std::endl;
		return false;
	} 

	return true;
}

void Response::getEtag(){
	size_t pos_Etag = response.find("ETag:");
	if(pos_Etag!=std::string::npos){
		std::string mid_Etag = response.substr(pos_Etag+9);
		size_t pos_Etag_end = mid_Etag.find("\r\n");
		Etag = mid_Etag.substr(0,pos_Etag_end);
	}
}


void Response::getLastModified(){
	size_t pos_LastModified = response.find("Last-Modified: ");
	if(pos_LastModified!=std::string::npos){
		std::string mid_LastModified = response.substr(pos_LastModified+9);
		size_t pos_LastModified_end = mid_LastModified.find("\r\n");
		LastModified = mid_LastModified.substr(0,pos_LastModified_end);
	}
}


void Response::parseResponse(){
	getHeader_body();
	getStatus();
	getTransferEncoding();
    getcontent_len();
    getCacheControl_MaxAge();
    getDate_convert();
   	getExpires_convert();
   	getEtag();
   	getLastModified();

}

