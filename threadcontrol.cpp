#include "threadcontrol.h"


bool proxy_control::recvFromClient(){
  int len = recv(new_socket, &clientbuff.data()[0], clientbuff.size(), 0);
  if(len < 0) {
    std::cerr << "fail to receive request" << std::endl;
    return false;
  }
  else if(len == 0) {
    std::cout << "No request!" << std::endl;
    return false;
    //goto rec;
  }
  else {
    std::cout << "Receive request: " << std::endl << clientbuff.data() << std::endl;
    return true;
  }
}

bool proxy_control::connectToServer(requestHead & reqHead) {
  int conn_status;
  int addr_status;
  struct addrinfo server_info;
  struct addrinfo * s;
  //char port[] = "80";
  memset(&server_info, 0, sizeof(server_info));
  server_info.ai_family   = AF_UNSPEC;
  server_info.ai_socktype = SOCK_STREAM;
  addr_status = getaddrinfo(reqHead.get_host().c_str(), reqHead.get_port().c_str(), &server_info, &server_info_list);
  //addr_status = getaddrinfo(reqHead.get_host().c_str(), port, &server_info, &server_info_list);
  //std::cout << addr_status;
  if(addr_status != 0) {
    std::cerr << "Error: fail to analyze ip address for server" << std::endl;
    //freeaddrinfo(server_info_list);
    return false;
  }
  for(s = server_info_list; s != NULL; s = s->ai_next){
    conn_status = -1;
    conn_socket = socket(s->ai_family,
		       s->ai_socktype,
		       s->ai_protocol);
    if(conn_socket < 0){
      std::cerr << "Error: fail to create a connect socket for proxy!" << std::endl;
      continue;
    }
    conn_status = connect(conn_socket, s->ai_addr, s->ai_addrlen);
    if(conn_status < 0){
      std::cerr << "Error: fail to connect server, try again!" << std::endl;
      //close(conn_status);
      continue;
    }
    break;
  }
  if(s == NULL) {
    std::cerr << "Error: fail to connect server!" << std::endl;
    //freeaddrinfo(server_info_list);
    return false;
  }
  std::cout << "Connecting to Server!" << std::endl;
  return true;
}

bool proxy_control::sendToServer() {
  int len = send(conn_socket, &clientbuff.data()[0], clientbuff.size() + 1, 0);
  if(len < 0){
    std::cerr << "fail to send request to server!" << std::endl;
    //close(conn_socket);
    return false;
  }
  else {
    std::cout << "HTTP Header send successfully! " << std::endl;
    return true;
  }
}

  //int len = 0;
  //size_t total_left = clientbuff.size();
  //std::cout << "left:" << total_left << std::endl;
  //size_t next_send = 0 ;
  //len = send(conn_socket, &clientbuff.data()[0], clientbuff.size() + 1, 0)
  /*while (total_left > 0){
    len = send(new_socket, &clientbuff.data()[next_send], total_left, 0);
    std::cout << "send back to client len:" << len << std::endl;
    if(len == -1) {
	std::cerr << "Error: send response back to client failed" << std::endl;
	return false;
    }
    if(len == 0) {
      break;
    }
    total_left-=len;
    next_send+=len;
    std::cout << "send back to client total_left:" << total_left << std::endl;
    std::cout << "send back to client next_send:" << next_send << std::endl;
    }*/
  /*while (next_send < clientbuff.size()){
    len = send(conn_socket, &clientbuff.data()[next_send], total_left, 0);
    //std::cout << "send back to client len:" << len << std::endl;
    if(len == -1) {
      return false;
   }
    total_left -= len;
    next_send += len;
    }*/
  //std::cout << "HTTP Header send successfully! " << std::endl;
  //return true;
  //}


bool proxy_control::recvFromServer(){
  int len;
  //std::vector<char> buff;
  //buff.resize(BUFF_SIZE);
  //std::fill(buff.begin(), buff.end(), '\0');
  //std::cout << buff.data() <<std::endl;
  int i = 0;
  //serverbuff.resize(BUFF_SIZE);
  //std::fill(serverbuff.begin(), serverbuff.end(), 0);
  //len = recv(conn_socket, &serverbuff.data()[0], BUFF_SIZE, 0 );
  
  do{
    std::vector<char> buff;
    buff.resize(BUFF_SIZE);
    std::fill(buff.begin(), buff.end(), '\0');
    //std::vector<char>::iterator it = serverbuff.begin() + serverbuff.size();
    len = recv(conn_socket, &buff.data()[0], BUFF_SIZE, 0);
    //    std::cout << i << ":buff:" << std::endl << buff.data() <<std::endl;
    //    std :: cout << "len:" << len << std::endl;
    if(len < 0){
      if(serverbuff.size() == 0){
	std::cerr << "Error: fail to receive response from server!" << std::endl;
	//close(conn_socket);
	//freeaddrinfo(server_info_list);
	//exit(EXIT_FAILURE);
	return false;
      }
      else{
	break;
      }
    }
    serverbuff.insert(serverbuff.end(), buff.begin(), buff.begin() + len);
    //serverbuff.insert(serverbuff.end(), '\0');
    //serverbuff.resize(serverbuff.size() + len);
    //std::cout <<"serverbuff:"  << std::endl << serverbuff.data() << std::endl;
    //std::cout <<"serverbuff:" << std::endl << serverbuff.data() << std::endl;
    //std::copy(buff.begin(), buff.begin() + len, it);
    i++;
  }while(len != 0);
  serverbuff.insert(serverbuff.end(), '\0');

  //  std::cout <<"serverbuff:"  << std::endl << serverbuff.data() << std::endl;
  //std::cout << "len: " << len <<std::endl;
  /*
  responseHead resHead;
  resHead.parseResponse(serverbuff);
  std::string tempKey = std::string(clientbuff.begin(),clientbuff.end());
  size_t headPos = tempKey.find("\r\n");
  std::string key = "";
  //std::cout << "-------------the url to cache--------------------"<< std::endl;
  //std::cout <<"clientbuff:"  << std::endl << clientbuff.data() << std::endl;  
  if(headPos != std::string::npos){
    std::cout << "headPos" << headPos <<std::endl;
    std::string key = std::string(tempKey, 0, headPos);
    std::cout << "-------------url key--------------------"<< std::endl;   
    std::cout << key << std::endl;
    std::cout << "-----------------"<<std::endl;
    //allocateCache(key, resHead);
  }else{
    std::cout << "cannot do cache, beacause cannot find url" << std::endl;
  }
  
  
  
  std::cout << "-------------Response Buff--------------------"<< std::endl;
  std::cout <<"serverbuff:"  << std::endl << serverbuff.data() << std::endl;
  */  
  std::cout << "receive response from server successfully!" << std::endl;
  return true;
  //close(conn_socket);
}

bool proxy_control::sendToClient(){
  int len = 0;
  int total_left = serverbuff.size();
  std::cout << "left:" << total_left << std::endl;
  int next_send = 0;
  while (total_left > 0){
    len = send(new_socket, &serverbuff.data()[next_send], total_left, 0);
    std::cout << "send back to client len:" << len << std::endl;
    if(len <= 0) {
      if(errno == EINTR) {
	len = 0;
      }
      else {
	std::cerr << "Error: send response back to client failed" << std::endl;
	return false;
	//close(new_socket);
	//exit(EXIT_FAILURE);
      }
    }
    total_left-=len;
    next_send+=len;
    std::cout << "send back to client total_left:" << total_left << std::endl;
    std::cout << "send back to client next_send:" << next_send << std::endl;
  }
  std::cout << "send response to client successfully!" <<std::endl;
  //close(new_socket);
  return true;
}
