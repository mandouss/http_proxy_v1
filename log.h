#ifndef _LOG_H
#define _LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include "parser.h"
#include "time.h"

class Log{
 public:
  std::fstream& log;
  Log(std::fstream &log): log(log){}
  ~Log(){}
  void recvRequest(int &uid, requestHead& head,std::string clientAddress);
  void sendRequest(int &uid, requestHead& head);
  void recvResponse(int &uid, responseHead& head, requestHead& head2);
  void sendResponse(int &uid, responseHead& head);
  void tunnelClosed(int &luid);
  void checkCache1(int &uid);
  void checkCache2(int &uid,int cacheExist, std::string expiretime);
  void allocateCache(int uid, int cacheAllocateFlag, responseHead& head);
  void note(int uid, std::string s1, std::string s2);
};


#endif
