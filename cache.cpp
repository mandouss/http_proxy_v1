#include "cache.h"
//#include "log.h"

char* getLoctime(){
  time_t curtime;
  struct tm* loc_time;
  curtime = time(NULL);
  loc_time = localtime(&curtime);
  char* curr = asctime(loc_time);
#ifdef DEBUG
  printf("getLoctime is:%s\n", curr);
#endif
  char* time = ( char* ) malloc(strlen(curr)+1);
  memset(time, '\0', strlen(curr)+1);
  strcpy(time, curr);
  return time; 
}

int main(){
  std::cout<< getLoctime();
}
