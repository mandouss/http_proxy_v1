#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#define BUFF_SIZE 104800
#include <string>
#include <vector>
#include <iostream>

int main(int argc, char const *argv[])
{
  std::vector<std::string> set;
  set.push_back("GET http://www.cplusplus.com/reference/queue/priority_queue/ HTTP/1.1\r\nHost: www.cplusplus.com\r\nProxy-Connection: Keep-Alive\r\n\r\n");
  set.push_back("GET http://www.sina.com.cn/ HTTP/1.1\r\nHost: www.sina.com.cn\r\nProxy-Connection: keep-alive\r\n\r\n");
  set.push_back("GET http://stackoverflow.com/questions/218384/what-is-a-nullpointerexception-and-how-do-i-fix-it HTTP/1.1\r\nHost: stackoverflow.com\r\nProxy-Connection: Keep-Alive\r\n\r\n");
  set.push_back("GET http://beej.us/guide/bgnet/html/multi/introi.html#audience HTTP/1.1\r\nHost: beej.us\r\nProxy-Connection: Keep-Alive\r\n\r\n");
  set.push_back("GET http://pubs.opengroup.org/onlinepubs/9699919799/functions/strlen.html HTTP/1.1\r\nHost: pubs.opengroup.org\r\nProxy-Connection: Keep-Alive\r\n\r\n");
  set.push_back("GET http://m.news.cctv.com/2018/03/02/ARTIHXhVz5kFB9oghLHQb0FQ180302.shtml HTTP/1.1\r\nHost: www.gnu.org\r\nProxy-Connection: Keep-Alive\r\n\r\n");
  set.push_back("GET http://man7.org/linux/man-pages/man3/pthread_create.3.html#DESCRIPTION HTTP/1.1\r\nHost: man7.org\r\nProxy-Connection: Keep-Alive\r\n\r\n");

  if(argc != 3){
    perror("Input port and No. of message\n");
    exit(EXIT_FAILURE);
  }
  int po = atoi(argv[1]);
  if(po < 1024){
    perror("Engaging reserved port is banned\n");
    exit(EXIT_FAILURE);
  }
  
  int choice = atoi(argv[2]);
  if(choice > 7 || choice < 0){
    perror("Wrong choice of send message\n");
  }
  std::vector<char> buff(set[choice - 1].begin(), set[choice - 1].end());
  
  struct sockaddr_in client;
  unsigned int socket_len = sizeof(client);

  /* Create socket */
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd < 0){
    perror("Failed to create socket\n");
    exit(EXIT_FAILURE);
  }

  memset(&client, 0, socket_len);
  client.sin_family = AF_INET;
  client.sin_port = htons(po);
  int ip_status = inet_pton(AF_INET, "127.0.0.1", &client.sin_addr);
  if(ip_status != 1){
    perror("Failed to construct socket\n");
    exit(EXIT_FAILURE);
  }

  /* Connect */
  int conn_fd = connect(socket_fd, (struct sockaddr *)&client, socket_len);
  if(conn_fd < 0){
    perror("Connection error\n");
    exit(EXIT_FAILURE);
  }
  printf("Build connection\n");
  int send_status = send(socket_fd, &buff.data()[0], BUFF_SIZE, 0);
  if(send_status < 0){
    perror("Failed to send message\n");
    exit(EXIT_FAILURE);
  }
  printf("Message is successfully sent\n");
  std::cout << buff.data() << std::endl;
}
