#include "parser.h"
//
//  parser.cpp
//
//
//  Created by xiaoyuli on 2018/2/26.
//

bool requestHead::parseRequest(std::vector<char> & buffer){
  //bool ans = false;
  // some code that places data into buffer
  char *buff = new char[buffer.size()];
  std::copy(buffer.begin(), buffer.end(), buff);
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
  start = curr + 1;
  curr = curr + 1;

  while(*newline != '\r'){
    newline++;
  }
  protocol = std::string(start, newline - start);


  start = newline + 8;
  newline = newline + 2;
  while(*newline != '\r'){
    newline++;
  }
  host = std::string(start, newline - start);

  std::cout<< "method: "<< method << std::endl;
  std::cout<< "path: " << path << std::endl;
  std::cout<< "protocol: " << protocol << std::endl;
  std::cout<< "host: " << host << std::endl;
  delete buff;
  return true;
}

bool responseHead::parseResponse(std::vector<char> & buffer){
  // some code that places data into buffer
  char *buff = new char[buffer.size()];
  std::copy(buffer.begin(), buffer.end(), buff);
  char* start = buff;
  char* end = strstr(start, "\r\n");
  char* line = end;
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
  std::cout<< "code: "<< code << std::endl;
  std::cout<< "status: " << status << std::endl;
  std::cout<< "date: " << date << std::endl;
  return true;
}
/*
int main(){
  char msg1[]= "GET /test HTTP/1.1\r\nHost: 192.241.213.46:6880\r\nUpgrade-Insecure-Requests: 1\r\n\r\n";
  char msg2[]= "HTTP/1.x 200 OK\r\nTransfer-Encoding: chunked\r\nDate: Sat, 28 Nov 2009 04:36:25 GMT\r\n\r\n";
  requestHead head1;
  head1.parseRequest(msg1);
  responseHead head2;
  head2.parseResponse(msg2);
  return EXIT_SUCCESS;

}
*/



