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

#define EXPIRE_TIME 10000.0

std::unordered_map<std::string, responseHead> cache;
std::string getCurrTime();
double timeDiff(std::string date1);
std::vector<char> checkCache(requestHead head, int uid, FILE* file);
int allocateCache(std::string url, responseHead head);
