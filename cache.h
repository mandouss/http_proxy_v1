#ifndef _CACHE_H
#define _CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <unordered_map>
#include "parser.h"
#include "time.h"

#define EXPIRE_TIME 10000.0
extern std::unordered_map<std::string, responseHead> cache;  
//std::string getCurrTime();
//extern double timeDiff(std::string date1);
int checkCache(requestHead& head);
int allocateCache(std::string url, responseHead& head);
/*
std::string getCurrTime(){
  time_t curtime;
  struct tm* loc_time;
  curtime = time(NULL);
  loc_time = localtime(&curtime);
  char* curr = asctime(loc_time);
  std::string ans(curr);
  return ans;
  }
*/
/*
double timeDiff(std::string date1){
  struct tm c;
  std::string curr1 = getCurrTime();
  const char *curr = curr1.c_str();
  strptime(curr, "%a, %b %d %H:%M:%S %Y", &c);

  struct tm d;
  const char *date = date1.c_str();
  strptime(date, "%a, %d %b %Y %H:%M:%S GMT", &d);
  time_t date_t = mktime(&d);
  time_t curr_t = mktime(&c);
  double diff = difftime(curr_t, date_t);

  //printf("curr - date in seconds is: %f\n", diff);

  return diff;
  }
*/
#endif
