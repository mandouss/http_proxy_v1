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
#include "parser.h"
#include "time.h"
class Log{
 public:
  std::fstream& log;
  Log(std::fstream &log): log(log){}
  ~Log(){
    log.close();
  }
  void recvRequest(int uid, requestHead& head);
  void sendRequest(int uid, requestHead& head);
  void recvResponse(int uid, responseHead& head, requestHead& head2);
  void sendResponse(int uid, responseHead& head);
  void tunnelClosed(int uid);
};

//std::string getCurrTime();
