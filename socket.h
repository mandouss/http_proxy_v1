#ifndef _SOCKET_H__
#define _SOCKET_H__

#include <cstring>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <fstream>
#include <sys/stat.h>
#include <stdbool.h>

#include "parser.h"
#include "threadcontrol.h"
#include "cache.h"
class proxySocket {
 private:
  int sockfd; //initial socket for proxy setup
  int new_socket; //function accpet() to client
  int conn_socket; //connect to web server
  //int port;
  struct addrinfo host_info; // c_addr, s_addr;
  struct addrinfo *host_info_list;
  struct sockaddr_in socket_addr;
 public:
  proxySocket();
  ~proxySocket();
  void get_host_info(char * port);
  void setupServer(char * port); //create, bind, listen
  void acceptConnect();
  struct addrinfo * get_host_info_list();
  //struct sockaddr_in get_c_addr();
  //int send(std::string);
  //int recv(std::string);
  int get_conn_socket ();
  int get_new_socket ();
  int get_sockfd ();
  struct sockaddr_in get_socket_addr();
};

#endif
