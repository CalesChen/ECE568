
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "response.h"


void response::getHeader_body(){
	size_t pos_header_end = response.find("\r\n\r\n");
	header = response.substr(0,pos_header_end);
	body = request.substr(pos_header_end+4);
}

void response::getStatus(){
	size_t pos_status = header.find(" ");
	std::string mid_status = header.substr(pos_status);
	size_t pos_status_end = mid_status.find("\r\n");
	status = mid_status.substr(0,pos_status_end);

}

void response::getTransferEncoding(){
	size_t pos_TransferEncoding = response.find("Transfer-Encoding:");
	if(pos_TransferEncoding!=string::npos){
		std::string mid_TransferEncoding = response.substr(pos_TransferEncoding);
		size_t pos_TransferEncoding_end = mid_TransferEncoding.find("\r\n");
		Transfer-Encoding = mid_TransferEncoding.substr(0,pos_TransferEncoding_end);
	}
}

void response::getcontent_len(){
	size_t pos_content_len = response.find("Content-Length:");
	if(pos_content_len!=string::npos){
		std::string mid_content_len = response.substr(pos_content_len);
		size_t pos_content_len_end = mid_content_len.find("\r\n");
		Transfer-Encoding = mid_content_len.substr(0,pos_content_len_end);
	}
}

void getCacheControl(){
	size_t pos_CacheControl = response.find("Transfer-Encoding:");
	if(pos_CacheControl!=string::npos){
		std::string mid_CacheControl = response.substr(pos_CacheControl);
		size_t pos_CacheControl_end = mid_CacheControl.find("\r\n");
		CacheControl = mid_CacheControl.substr(0,pos_CacheControl_end);
	}
}


time_t response::getDate_convert(){
	size_t pos_date = response.find("Date:");
	if(pos_date!=string::npos){
		std::string mid_date = response.substr(pos_date);
		size_t pos_date_end = mid_date.find("\r\n");
		date = mid_date.substr(0,pos_date_end);
	}
	tm tm_;
	strptime(date.c_str(), "%Y-%m-%d %H:%M:%S", &tm_);
	convertedDate  = mktime(&tm_);
	return convertedDate;
}

time_t response::getExpires_convert(){
	size_t pos_expires = response.find("Expires:");
	if(pos_expires!=string::npos){
		std::string mid_expires = response.substr(pos_expires);
		size_t pos_expires_end = mid_expires.find("\r\n");
		Expires = mid_expires.substr(0,pos_expires_end);
	}

	tm tm_;
	strptime(Expires.c_str(), "%Y-%m-%d %H:%M:%S", &tm_);
	convertedExpires  = mktime(&tm_);
	return convertedExpires;
}

bool response::timeValid(){
	if(convertedDate>convertedExpires){
		return false;
	} else {
		return true;
	}
}

void response::parseResponse(){
	getHeader_body();
	getStatus();
	getTransferEncoding();
    getcontent_len();
    getCacheControl();
    getDate_convert();
   	getExpires_convert();

}

