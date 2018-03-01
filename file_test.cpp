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

using namespace std;

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
  logfile << "Hello World!" << std::endl;
  std::cout << "Hello\n";
  logfile << "You are young!" <<std::endl;
  logfile.close();
}
