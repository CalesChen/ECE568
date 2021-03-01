#include <iostream>
#include <string>
  
#include "request_info.h"


void request_info::getHeader_body_contentLen(){
	size_t pos_header_end = request.find("\r\n\r\n");
	header = request.substr(0,pos_header_end);
	// if(method == "GET"){
	// 	body = NULL;
	// } else 
	if(method == "POST"){
		body = request.substr(pos_header_end+4);
		size_t pos_contentLen = header.find("Content-Length");
		std::string mid_contentLen = header.substr(pos_contentLen+16);
		size_t pos_contentLen_end = mid_contentLen.find("\r\n");
		content_len = mid_contentLen.substr(0,pos_contentLen_end);
	}
}

void request_info::getRequestLine(){
	size_t line_end = request.find("\r\n");
	request_line = request.substr(0,line_end);

}

void request_info::getMethod_URI(){
	size_t pos_method = request_line.find(" ");
	method = request_line.substr(0,pos_method);
	size_t pos_url = request_line.find(" ",pos_method+1);
	uri = request_line.substr(pos_method+1, pos_url - pos_method);
}

void request_info::getHost_port(){
	size_t pos_host = request.find("Host");
	std::string mid_host = request.substr(pos_host+6);
	size_t end = mid_host.find("\r\n");
	std::string host_port = mid_host.substr(0,end);
	size_t port_pos = host_port.find(":\r");
	if(port_pos!= std::string::npo){
		host = host_port.substr(0,port_pos);
		port = host_port.substr(port_pos+1);
	} else{
		host = host_port;
		port="80";
	}
	//?????
}

void request_info::parseRequest(){
	getHeader_body_contentLen();
	getRequestLine();
	getMethod_URI();
	getHost_port();
}