#include "socket.h"

pthread_mutex_t lock;
std::unordered_map<std::string, responseHead> cache;

proxySocket::proxySocket(): sockfd(-1), new_socket(-1), conn_socket(-1){
  memset(&host_info, 0, sizeof(host_info));
  host_info_list = NULL;
  //memset(&s_addr, 0, sizeof(s_addr));
}

proxySocket::~proxySocket(){
  freeaddrinfo(host_info_list);
  close(sockfd);
  close(new_socket);
  //close(conn_socket);
}

int proxySocket::get_conn_socket() {
  return conn_socket;
}

int proxySocket::get_new_socket() {
  return new_socket;
}

int proxySocket::get_sockfd() {
  return sockfd;
}

struct addrinfo * proxySocket::get_host_info_list(){
  return host_info_list;
}

struct sockaddr_in proxySocket::get_socket_addr(){
  return socket_addr;
}

void proxySocket::get_host_info(char * port) {
  int status;
  const char *hostname = NULL;
  host_info.ai_family   = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  host_info.ai_flags    = AI_PASSIVE;
  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if(status != 0) {
    std::cerr << "Error: fail to analyze ip address for host" << std::endl;
    freeaddrinfo(host_info_list);
    exit(EXIT_FAILURE);
  }
}

void proxySocket::setupServer(char * port) {
  int optval = 1;
  int back_log = 20;
  int status = -1;
  sockfd = socket(host_info_list -> ai_family,
		  host_info_list -> ai_socktype,
		  host_info_list -> ai_protocol);
  if(sockfd < 0){
    std::cerr << "Error: fail to create a proxy socket for listening!" <<std::endl;
    exit(EXIT_FAILURE);
  }
  if((status = bind(sockfd, host_info_list->ai_addr, host_info_list->ai_addrlen)) < 0) {
    std::cerr << "fail to bind proxy port and socket!" << std::endl;
    close(sockfd);
    exit(EXIT_FAILURE);
  }
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));//set socket reuse
  status = -1;
  if((status = listen(sockfd, back_log)) < 0){ // listen
    std::cerr << "fail to bind proxy port and socket!" << std::endl;
    close(sockfd);
    exit(EXIT_FAILURE);
  }
}

void proxySocket::acceptConnect() {
  std::cout << "Waiting for connection on port!" <<std::endl;
  //struct sockaddr_storage socket_addr;
  socklen_t socket_addr_len = sizeof(socket_addr);
  new_socket = accept(sockfd, (struct sockaddr *)&socket_addr, &socket_addr_len);
  if(new_socket < 0) {
    std::cerr << "fail to call accept function!" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << "begin connect!" << std::endl;
}

void * multiThreadHelper(void * arg_list){
  thread_control* thread= ((thread_control *)arg_list);
  int new_socket_tmp = ((thread_control *)arg_list) -> new_socket_t;
  bool conn_status; // check connectToServer success or failure
  bool sendserver_status;
  struct sockaddr_in server_host_tmp = ((thread_control *)arg_list) -> server_host_t;
  proxy_control sc(new_socket_tmp, server_host_tmp, (((thread_control *)arg_list) -> logfile).log);
  Log & logfile = thread->logfile;
  int & uid = thread->get_uid();
  requestHead reqHead;
  responseHead respHead;
  std::string typeFlag = "";
  int cacheExist = 0;
  int cacheAllocateFlag = 0;
  if(!sc.recvFromClient()){
    std::cerr << "Error: Connection closed!" << std::endl;
    pthread_exit(NULL);
  }
  reqHead.parseRequest(sc.get_clientbuff());
  
  //sc.get_clientbuff();
  
  pthread_mutex_lock(&lock);
  //sc.logGetRequest();
  std::string clientAddress(inet_ntoa(server_host_tmp.sin_addr));
  logfile.recvRequest(uid, reqHead, clientAddress); 

  typeFlag = reqHead.get_method();
  std::string expiretime ="";

  if(typeFlag == "GET"){
    cacheExist = checkCache(reqHead);
    std::cout <<"--------cacheExist------ : " << cacheExist <<std::endl;
    if(cacheExist == 1){
      std::cout <<"---------use cache to do return to client"<<std::endl;
      for(auto it : cache[reqHead.get_head()].get_body()){
	sc.get_serverbuff().push_back(it);
      }
      if(!sc.sendToClient()) {
	std::cerr << "Error: Connection closed!" << std::endl;
	pthread_exit(NULL);
      }
      logfile.sendResponse(uid, respHead);
      logfile.checkCache1(uid);
      std::cout << "Success! Connection closed!" <<std::endl;
      logfile.tunnelClosed(uid);
      uid++;
      pthread_mutex_unlock(&lock);
      pthread_exit(NULL);
    }else if(cacheExist == 2){
      
      if(cache.find(reqHead.get_head()) != cache.end()){
	expiretime = cache[reqHead.get_head()].get_expire();
      }
      logfile.checkCache2(uid,cacheExist,expiretime); 
    }else if(cacheExist == 3){
      logfile.checkCache2(uid,cacheExist,expiretime);
    }else if(cacheExist == 4){
      logfile.checkCache2(uid,cacheExist, expiretime);
    }else{
      std::cerr << "Error: Do not have request method! " << std::endl;
      pthread_exit(NULL); 
    }
  }
  
  conn_status = sc.connectToServer(reqHead);
  if(conn_status == false){
    std::cerr << "Error: Connection closed! " << std::endl;
    pthread_exit(NULL);
  }
  sendserver_status = sc.sendToServer();
  if(sendserver_status == false) {
    std::cerr << "Error: Fail to send request to server. Connection closed!" << std::endl;
    pthread_exit(NULL);
  }
  logfile.sendRequest(uid, reqHead); 
  if(!sc.recvFromServer()) {
    std::cerr << "Error: Receive from server failed. Connection closed!"  <<std::endl;
    pthread_exit(NULL);
  }
  
  respHead.parseResponse(sc.get_serverbuff());
  logfile.recvResponse(uid, respHead, reqHead); 
  //  std::cout <<"-------------show me the cache";
  //std::cout << typeFlag;
  //std::cout << respHead.get_status();
  if(typeFlag == "GET" && respHead.get_status() == "OK"){
    logfile.note(uid, "Here we try to allcate cache", respHead.get_head());
    cacheAllocateFlag = allocateCache(reqHead.get_head(), respHead);
    logfile.allocateCache(uid, cacheAllocateFlag, respHead);
  }

  
  if(!sc.sendToClient()) {
    std::cerr << "Error: Connection closed!" << std::endl;
    pthread_exit(NULL);
  }
  logfile.sendResponse(uid, respHead);
  
  std::cout << "Success! Connection closed!" <<std::endl;
  //close(sc.get_new_socket());
  //std::cout << sc.get_socket() << std::endl;
  //close(sc->get_new_socket());
  //delete sc
  logfile.tunnelClosed(uid);
  uid++;
  //thread->increase_uid();
  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
  int p_port;
  if(argc != 2) {
    std::cerr << "USage: ./socket <port_num>" <<std::endl;
    exit(EXIT_FAILURE);
  }
  p_port = atoi(argv[1]);
  if(p_port < 1024) {
    std::cerr << "Error: Please input a port larger than 1024!" << std::endl;
    exit(EXIT_FAILURE);
  }
  proxySocket p_socket;
  //file proxy.log
  int dict_status = access("/var/log/erss/", 00);
  if(dict_status == -1) {
    if(mkdir("/var/log/erss/", 0777) == -1){
      std::cerr << "fail to create dict! Please run with 'sudo'. " <<std::endl; 
      exit(EXIT_FAILURE);
    }
  }
  std::fstream lf;
  lf.open("/var/log/erss/proxy.log", std::ios::in | std::ios::out | std::ios::app);
  if(!lf){
    std::cerr << "fail to create proxy.log!" <<std::endl;
    exit(EXIT_FAILURE);
  }
  Log logFile(lf);
  int uid = 0;
  //function for proxy setup as a Server
  p_socket.get_host_info(argv[1]);
  p_socket.setupServer(argv[1]);

  //open background work
  int daemon_flag = daemon(0, 1);
  if(daemon_flag!= 0) {
    std::cerr << "Error: fail to create Daemon!" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  while(1){
    p_socket.acceptConnect();
    thread_control arg_list(p_socket.get_new_socket(),p_socket.get_socket_addr(), logFile, uid);
    pthread_t thread;
    if(pthread_create(&thread, NULL, multiThreadHelper, &arg_list) != 0) {
      std::cerr << "fail to create thread!" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  lf.close();
  //std::cout << p_socket.get_Sockfd() << std::endl;
  //std::cout << p_socket.get_S_Socket() << std::endl;
}

