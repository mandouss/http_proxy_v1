#include "log.h"

void Log::recvRequest(int &uid, requestHead& head, std::string clientAddress){
  time_t curtime;
  struct tm* loc_time;
  curtime = time(NULL);
  loc_time = gmtime(&curtime);
  char* curr = asctime(loc_time);
  std::string ans(curr);
  
  log << uid << ": \""<< head.get_head()<< "\" from "<< clientAddress << " @ "<< ans;
}


void Log::sendRequest(int &uid, requestHead& head){
  log << uid << ": Requesting \""<< head.get_head()<< "\" from "<< head.get_host()<< std::endl;
}

void Log::recvResponse(int &uid, responseHead& head, requestHead& head2){
  log << uid << ": Received \""<< head.get_head()<< "\" from "<< head2.get_host()<<std::endl;
  if(head.get_cache() != ""){
    log << uid << ": NOTE Cache-Control: " << head.get_cache()<< std::endl;
  }
  if(head.get_etag() != ""){
    log << uid << ": NOTE Etag: " << head.get_etag()<< std::endl;
  }
}

void Log::sendResponse(int &uid, responseHead& head){
  log << uid << ": Responding "<< head.get_head()<<std::endl;
}

void Log::tunnelClosed(int &uid){
  log << uid << ": Tunnel closed"<<std::endl;
}

void Log::checkCache1(int &uid){
  log << uid << "in cache, valid" << std::endl;
}
void Log::checkCache2(int &uid,int cacheExist, std::string expiretime){
  if(cacheExist == 2){
    
    log << uid << "in cache, but expired at" << expiretime << std::endl;
  }else if(cacheExist == 3){
    log << uid << "in cache, requires validation " << std::endl;
  }else if(cacheExist == 4){
    log << uid << ": not in cache" << std::endl;
  }else{
    std::cerr << "not valid cacheflag here";
  }
}
void Log::allocateCache(int uid, int cacheAllocateFlag, responseHead& head){
  if(cacheAllocateFlag == 1){
    log << uid << ": not cacheable because " << head.get_cache() << std::endl;
  }else if(cacheAllocateFlag == 2){
    log << uid << ": cached, expires at " << head.get_expire() << std::endl;;
  }else if(cacheAllocateFlag == 3){
    log << uid << ": cached, but requires re-validation " <<std::endl;
  }else if(cacheAllocateFlag == 4){
    log << uid << "Note: No details about cache, choose not to cache" <<std::endl;
  }
}

void Log::note(int uid, std::string s1, std::string s2){
  log << uid << ": NOTE " << s1 << " " << s2 << std::endl;
}


/*
int main() {
  int dict_status = access("/var/log/erss/", 00); //
  if(dict_status == -1) {
    if(mkdir("/var/log/erss/", 0777) == -1){
      exit(EXIT_FAILURE);
    }
  }
  std::fstream logfile;
  logfile.open("/var/log/erss/proxy.log", std::ios::in | std::ios::out | std::ios::app);  
  if(!logfile){
    std::cerr << "fail to create proxy.log!" << std::endl;
    return 1;
  }

  Log logFile(logfile);
  logFile.log << "Hello World!" << std::endl;
  std::cout << "Hello\n";
  logFile.log << "You are young!" <<std::endl;
  
  const char *msg1  = "GET /test HTTP/1.1\r\nHost: 192.241.213.46:6880\r\nUpgrade-Insecure-Requests: 1\r\n\r\n";
  const char *msg2= "HTTP/1.x 200 OK\r\nTransfer-Encoding: chunked\r\nDate: Sat, 28 Nov 2009 04:36:25 GMT\r\n\r\n";
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

  logFile.recvRequest(1, head1);
  logFile.sendRequest(1, head1);
  logFile.recvResponse(1, head2, head1);
  logFile.sendResponse(1, head2);
  logFile.tunnelClosed(1);

}
*/
