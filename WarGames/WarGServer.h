#ifndef _WARG_SERVER_
#define _WARG_SERVER_

#include <iostream>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <ctime>
#include <fstream>

#include "Client.h"

using namespace std;
class WarGServer{

private:
		int serv,port;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	//int n;		
	  fd_set master;
  fd_set read_fds;
  int fdmax;
 std::vector<Client> clients;
    vector<Client> *clients_ptr;

public:
	void start();
     int handle_client(int);
     void close_connection(int);
     int getClient(int);
     void send_data(int,string);
     void send_data_delay(int,string,Client &c);
     Client &getClient2(int);

     void writeLog(string);
};

#endif