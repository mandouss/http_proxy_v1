#ifndef _THREADCONTROL_H__
#define _THREADCONTROL_H__

#include "header.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <vector>
#include <iostream>
#include "parser.h"
//#include "cache.h"
#include <errno.h>
#include "time.h"
#include "log.h"
#define BUFF_SIZE 104800

//extern std::unordered_map<std::string, responseHead> cache; 
/*struct _thread_arg{
  int new_socket_t; //thread's sockfd for browers
  struct sockaddr_storage server_host_t;
  };typedef struct _thread_arg thread_arg; */

class thread_control{
 public:
  int new_socket_t;
  struct sockaddr_in server_host_t;
  //std::fstream logfile;
  Log& logfile;
  int &uid;
 thread_control(int s, struct sockaddr_in sht, Log& lf, int &uid): new_socket_t(s), server_host_t(sht), logfile(lf),uid(uid) {}
  //thread_control(int s, struct sockaddr_in sht, std::fstream lf): new_socket_t(s), server_host_t(sht, logfile(lf));
  ~thread_control(){}
  void increase_uid(){
    uid++;
  }
  int & get_uid(){
    return uid;
  }
};

class proxy_control{
 private:
  int new_socket; //for browser
  int conn_socket;
  struct sockaddr_in server_host; //proxy
  struct addrinfo * server_info_list;
  std::vector<char> clientbuff;
  std::vector<char> serverbuff;
  //int uid;
  std::fstream &logfile;
 public:
 proxy_control(int s, struct sockaddr_in hil, std::fstream &lf): new_socket(s), conn_socket(-1), server_host(hil), logfile(lf) {
    clientbuff.resize(BUFF_SIZE);
    //serverbuff.resize(sizeof(char));
    std::fill(clientbuff.begin(), clientbuff.end(), '\0');
    //std::fill(serverbuff.begin(), serverbuff.end(), 0);
  }  //initialize req_str
  ~proxy_control() {
    close(conn_socket);
    //close(new_socket);
    freeaddrinfo(server_info_list);
  }
  int get_new_socket() {
    return new_socket;
  }
  int get_conn_socket() {
    return conn_socket;
  }
  struct sockaddr_in get_server_host(){
    return server_host;
  }
  std::vector<char> get_clientbuff(){
    return clientbuff;
  }
  std::vector<char> get_serverbuff(){
    return serverbuff;
  }
  std::fstream & get_logfile() {
    return logfile;
  }
  /*
  int get_uid() {
    return uid;
  }
  void increase_uid(){
    uid++;
  }
  */
  bool recvFromClient();
  bool connectToServer(requestHead & reqHead);
  bool recvFromServer();
  bool sendToClient();
  bool sendToServer();
};

#endif
