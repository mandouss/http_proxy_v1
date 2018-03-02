#include "parser.h"
// right
//  parser.cpp
//
//
//  Created by xiaoyuli on 2018/2/26.
//

bool requestHead::parseRequest(std::vector<char> buffer){
  //bool ans = false;
  // some code that places data into buffer
  char *buff = new char[buffer.size() + 1];
  std::copy(buffer.begin(), buffer.end(), buff);
  buff[buffer.size()] = '\0';

  const char *start = buff;
  const char *curr = buff;
  const char *newline = buff;

  while(*curr != ' '){
    curr++;
  }

  method = std::string(start, curr - start);
  start = curr + 1;
  curr = curr + 1;
  while(*curr != ' '){
    curr++;
  }

  path = std::string(start, curr - start);
  if(path.find("http://") != std::string::npos || path.find("Http://") != std::string::npos){
    path = std::string(path, 7);
  }
  size_t colon;
  size_t slash;
  //std::cout << "here1";
  if((colon = path.find(':')) != std::string::npos){
    //std::cout << "here2";
    if((slash = path.find('/', colon)) == std::string::npos){
      port = std::string(path, colon + 1);
    }else{
      port = std::string(path, colon + 1, slash - colon - 1);
    }

  }else{
    //std::cout << "here3";
    port = "80";
  }
  start = curr + 1;
  curr = curr + 1;
  

  while(*newline != '\r'){
    newline++;
  }
  protocol = std::string(start, newline - start);
  head = std::string(buff, newline - buff);

  start = newline + 8;
  newline = newline + 8;
  while(*newline != '\r'){
    newline++;
  }
  host = std::string(start, newline - start);

  if((colon = host.find(':')) != std::string::npos){
    if((slash = host.find('/', colon)) == std::string::npos){
      host = std::string(host, 0, colon);
    }else{ 
      host = std::string(host, 0, colon);
    } 
  }

  std::cout<< "method: "<< method << std::endl;
  std::cout<< "path: " << path << std::endl;
  std::cout<< "protocol: " << protocol << std::endl;
  std::cout<< "host: " << host << std::endl;
  std::cout<< "head: " << head << std::endl;
  std::cout << "port: " << port << std::endl;
  std::cout << "-------------"<< std::endl;  
  delete[] buff;
  return true;
}

bool responseHead::parseResponse(std::vector<char> buffer){
  // some code that places data into buffer
  for(char it : buffer){
    body.push_back(it);
  }
  std::string temp = std::string(buffer.begin(),buffer.end());
  char *buff = new char[buffer.size() + 1];
  std::copy(buffer.begin(), buffer.end(), buff);
  buff[buffer.size()] = '\0';
  char* start = buff;
  char* end = strstr(start, "\r\n");
  char* line = end;
  head = std::string(start, line - start);
  start = strchr(start, ' ') + 1;
  end = strchr(start, ' ');
  code = std::string(start, end - start);

  start = strchr(start, ' ') + 1;
  end = strstr(start, "\r\n");
  status = std::string(start, end - start);

  if((start = strstr(line, "\r\nDate: ")) != NULL){
    //std::cout << start;
    start = strchr(start, ' ') + 1;
    end = strstr(start, "\r\n");
    date = std::string(start, end - start);
  }else{
    date = "";
  }
    if((start = strstr(line, "\r\nExpires: ")) != NULL){
    start = strchr(start, ' ') + 1;
    end = strstr(start, "\r\n");
    expire = std::string(start, end - start);
  }else{
    expire = "";
  }

  if((start = strstr(line, "\r\nContent-Length: ")) != NULL){                  

    start = strchr(start, ' ') + 1;
    end = strstr(start, "\r\n");
    length = std::string(start, end - start);
  }else{
    length = "";
  }
  if((start = strstr(line, "\r\nCache-Control: ")) != NULL){                  
    start = strchr(start, ' ') + 1;
    end = strstr(start, "\r\n");
    cache = std::string(start, end - start);
  }else{
    cache = "";
  }
  /*
  size_t temp1 = 0;
  size_t start1, end1;
    if( (temp1 = temp.find("\r\nEtag: ")) != std::string::npos){
      std::cout << "find Etag";
      start1 = temp.find(" ", temp1) + 1;
      end1 = temp.find("\r\n", start1);
      etag = std::string(temp, start1, end1 - start1);
    }
    std::cout << "start of Etag" << temp1 <<std::endl;    
    std::cout << "--------the string we get in parser--------"<< temp;
  */
  if((start = strstr(line, "\r\nEtag:")) != NULL){                  
    //std::cout << "find Etag"<< std::endl;
    start = strchr(start, ' ') + 1;
    end = strstr(start, "\r\n");
    etag = std::string(start, end - start);
  }else if((start = strstr(line, "\r\nETag:")) != NULL){
    start = strchr(start, ' ') + 1;
    end = strstr(start, "\r\n");
    etag = std::string(start, end - start);
  }else{
    etag = "";
  }
  
  std::cout << "-------------"<< std::endl;
  std::cout<< "code: "<< code << std::endl;
  std::cout<< "status: " << status << std::endl;
  std::cout<< "date: " << date << std::endl;
  std::cout<< "head: " << head << std::endl;
  std::cout<< "lenght: " << length << std::endl;
  std::cout<< "cache: " << cache << std::endl;
  std::cout<< "expire: " << expire << std::endl;
  std::cout<< "age: " << age << std::endl;
  std::cout<< "etag: " << etag << std::endl;
  std::cout << "-------------"<< std::endl;
  delete [] buff;
  return true;
}
/*
int main(){
  const char *msg1  = "GET /test HTTP/1.1\r\nHost: 192.241.213.46:6880\r\nUpgrade-Insecure-Requests: 1\r\n\r\n";
  const char *msg2= "HTTP/1.x 200 OK\r\nTransfer-Encoding: chunked\r\nDate: Sat, 28 Nov 2009 04:36:25 GMT\r\n\r\n";
  //  const char *msg1= "POST http://ocsp.digicert.com/ HTTP/1.1\r\nHost: ocsp.digicert.com\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:58.0) Gecko/20100101 Firefox/58.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nContent-Length: 83\r\nContent-Type: application/ocsp-request\r\nConnection: keep-alive\r\n\r\n";
  
  std::vector<char> vec1;
  const char* end1 = msg1 + strlen(msg1);
  vec1.insert(vec1.end(), msg1, end1);
  std::vector<char> vec2;
  const char* end2 = msg2 + strlen(msg2);
  vec2.insert(vec2.end(), msg2, end2);
  requestHead head1;
  head1.parseRequest(vec1);
  responseHead head2;
  head2.parseResponse(vec2);
  return EXIT_SUCCESS;
}
*/

