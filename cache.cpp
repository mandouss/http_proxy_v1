#include "cache.h"

//v1


int checkCache(requestHead& head){
  //std::vector<char> ans;
  //  int status;
      
  if(cache.find(head.get_head()) == cache.end()){
    return 4;
  }else{
    bool haveExpire = false; 
    time_t curtime;
    struct tm* loc_time;
    curtime = time(NULL);
    loc_time = localtime(&curtime);
    char* curr2 = asctime(loc_time);
    std::string ans(curr2);
  
    
    std::string date1 = cache[head.get_head()].get_expire();
    if(date1 != ""){
      haveExpire = true;
    }
    
    struct tm c;
    std::string curr1 = ans;
    const char *curr = curr1.c_str();
    strptime(curr, "%a, %b %d %H:%M:%S %Y", &c);

    struct tm d;
    const char *date = date1.c_str();
    strptime(date, "%a, %d %b %Y %H:%M:%S GMT", &d);
    time_t date_t = mktime(&d);
    time_t curr_t = mktime(&c);
    double diff = difftime(curr_t, date_t);
    std::cout << "have expire time" << date1 << std::endl;
    std::cout << "curr time" << ans << std::endl;
    std::cout << "Time difference we calculate" << diff << std::endl;
    //printf("curr - date in seconds is: %f\n", diff);
        
    std::string cache_value = cache[head.get_head()].get_cache();
    bool needValid  = false;
    if(cache_value != ""){
      if(cache_value.find("revalidate") == std::string::npos
	 || cache_value.find("age") == std::string::npos){
	needValid = true;
      }
    }
    if(needValid == true){
      return 3;
    }
    else if(haveExpire == true){
      
      if(diff > 0){
      //status = 2;
      printf("in cache, but expired");
      std::cout << diff << std::endl;
      //return 2; //"in cache, but expired at"
      }else{
	std::cout << "not sure about the cache validation, so do not use cache";
	//return 3;
      }
      return 2;
    }
    else{
      return 1;  //" in cache, valid"
    }
  }
  

  return -1; 
}


int allocateCache(std::string url, responseHead& head){
  std::string cacheControl = head.get_cache();
  std::string expire = head.get_expire();
  int status = 0;
  if(cacheControl.find("no-cache") != std::string::npos||
     cacheControl.find("no-store") != std::string::npos){
    status = 1;
    //return status;
  }else if(cacheControl.find("must-revalidate") != std::string::npos){
    status = 3;
    cache[url] = head;
    //return status;
  }else if(expire != ""){
    status = 2;
    cache[url] = head;
  }else{
    status = 4;
    std::cerr << "Do not have details about cache, so we do not cache"<<std::endl;
  }
  std::cout << "-----------demonstrate cache key ------------"<<std::endl;
  for(auto it = cache.begin(); it != cache.end(); it++){
    std::cout << "key: "<<it->first<<std::endl;
    std::cout << "expire: " << it->second.get_expire()<<std::endl; 
  }
  return status;
}                    
/*
int main(){
  getCurrTime();
}
*/

