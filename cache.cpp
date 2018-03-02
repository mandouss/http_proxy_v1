#include "cache.h"

//v1

/*
std::vector<char> checkCache(requestHead head, int uid, FILE* file){
  std::vector<char> ans;
  //  int status;
  if(cache.find(head.get_path()) == cache.end()){
    //status = 1;
    printf("not in cache");

  }else{

    //responseHead
    std::string date = cache[head.get_path()].get_date();
    if(timeDiff(date) > EXPIRE_TIME){
      //status = 2;
      printf("in cache, but expired at EXPIREDTIME");

    }
    //still need to test validation
    else{
      //      status = 4;
      printf("in cache, valid");
      ans = cache[head.get_path()].get_body();
    }
  }
  //  logCheckRequest(file, uid, status);

  return ans;
}
*/

int allocateCache(std::string url, responseHead head){
  std::string cacheControl = head.get_cache();
  int status = 2;
  if(cacheControl.find("no-cache") != std::string::npos||
     cacheControl.find("no-store") != std::string::npos){
    status = 1;
    return status;
  }
  cache[url] = head;

  if(cacheControl.find("must-revalidate") != std::string::npos){
    status = 3;
    cache[url] = head;
    return status;
  }
  std::cout << "-----------demonstrate cache key ------------"<<std::endl;
  for(auto it = cache.begin(); it != cache.end(); it++){
    std::cout << it->first<<std::endl;
  }
  return status;
}                    
/*
int main(){
  getCurrTime();
}
*/

