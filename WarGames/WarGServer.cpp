#include "WarGServer.h"

void WarGServer::start(){
	clients_ptr=&clients;
	serv=socket(AF_INET,SOCK_STREAM,0);
	if(serv<0){ cout<<"sock error\n"; return;}

	memset(&serv_addr,sizeof serv_addr,0);	

	port=9898;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port =htons(port);
		

	if(bind(serv,(struct sockaddr*)&serv_addr,sizeof serv_addr)<0){
		cout<<"error binding\n"; return;
		}

	listen(serv,5);
	FD_SET(serv,&master);
	fdmax=serv;
	clilen=sizeof cli_addr;
	int newsock;
	while(true){
	read_fds=master;
		if(select(fdmax+1,&read_fds,NULL,NULL,NULL)==-1){
			cout<<"select error\n";
			return;		
		}
		for(int i=0;i<=fdmax;i++){
		 if(FD_ISSET(i,&read_fds)){//data to be red
			if(i==serv){//new connection
			if((newsock=accept(serv,(struct sockaddr*)&cli_addr,&clilen))!=-1){
			      char ip[INET_ADDRSTRLEN];
			      inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,ip,INET_ADDRSTRLEN);
			      stringstream ss;
			      ss<<"Connection from "<<ip<<" accepted.";

			      writeLog(ss.str());


			      Client cli(newsock);
			      cli.setIp((string)ip);
			       clients.push_back(cli);
 	                	FD_SET(newsock,&master);
				if(newsock >fdmax){
				fdmax=newsock;
				}
				cout<<"new connection, "<<newsock<<endl;
			//	send_welcome(newsock);
				send_data(newsock,"LOGON: ");
			}else{
		              cout<<"error accepting\n";
			  //close(newsock);
		         }		
			}else{ //handle data from client
			  if(handle_client(i)==0){ //connection lost?
			  	  close_connection(i);
			   }
			}

		
		}
	}//for
	}//while
}



int WarGServer::handle_client(int sock){
int cli=getClient(sock);

	char buffer[256];
	int bytes;
    
	bytes=recv(sock,buffer,255,0);
	buffer[bytes-2]='\0';

		
		if(clients[cli].isSendingData()){
			
				
			return bytes;
		}
         string buf=(string)buffer;
	  if(buf=="Joshua" && !clients[cli].isLoggedin()){
	  	clients[cli].setLoggedin(true);
	  	send_data_delay(sock,"\r\nGREETINGS PROFESSOR FALKEN.\r\n",getClient2(sock));
	  	return bytes;
	  }

	  if(buf=="help games"){
        send_data_delay(sock,"\r\n'GAMES' REFERS TO MODELS, SIMULATIONS AND GAMES WHICH HAVE TACTICAL AND STRATEGIC APPLICATIONS.\r\n",getClient2(sock));
	 	return bytes;

	  }
 if(buf=="list games"){
 	string gameslst="\r\nFALKEN'S MAZE\r\nBLACK JACK\r\nGIN RUMMY\r\nHEARTS";
 	      gameslst+="\r\nBRIDGE\r\nCHECKERS\r\nCHESS\r\nPOKER";
	  	gameslst+="\r\nFIGHTER COMBAT\r\nGUERRILLA ENGAGEMENT\r\nDESERT WARFARE";
	   gameslst+="\r\nAIR-TO-GROUND ACTIONS\r\nTHREATERWIDE TACTICAL WARFARE";
	   gameslst+= "\r\nTHEATERWIDE BIOTOXIC AND CHEMICAL WARFARE";
	  gameslst+="\r\n\nGLOBAL THERMONUCLEAR WAR\r\n\n";
        send_data_delay(sock,gameslst,getClient2(sock));
	  return bytes;
	  
	  	}

 if(buf=="exit"){
 send_data(sock,"\r\nGoodBye\r\n");
 close_connection(sock);
 }


	if(!clients[cli].isLoggedin()){
      send_data(sock,"LOGON: ");
	}
	


	if(bytes>0){
		if(clients[cli].isLoggedin()){
		
		 send_data(sock,">");	
		}
	}	
	return bytes;
	
}


int WarGServer::getClient(int sock){
	int i=0;
	 for(i=0;i<clients.size();i++){
		if(clients[i].sck()==sock){
			return i;
		}
	}	
	return -1;
}
Client & WarGServer::getClient2(int sock){
	int i=0;
	 for(i=0;i<clients.size();i++){
		if(clients[i].sck()==sock){
			
			return clients[i];
		}
	}	
	//return NULL;
}


void WarGServer::send_data(int sock,string msg){



	char *data=new char[msg.length()+1];
	strcpy(data,msg.c_str());
	send(sock,data,msg.length(),0);
}
void WarGServer::send_data_delay(int sock,string msg,Client &c){
		//int c=getClient(sock);
	//	clients[c].setSendingData(true);

		pid_t f=fork();
		if(f==0){
    	
			c.setSendingData(true);
			double lt,ct;
                lt=(double)(clock())/10;
					lt+=5000;
			for(int i=0;i<msg.length();){
				 ct=(double)(clock())/10;
				if(ct-lt>=5000){
				char *data=new char[2];
				strcpy(data,msg.substr(i,1).c_str());
	            send(sock,data,1,0);
			    delete data;
			    lt=ct;
			    	i++;
			}
		}
		c.setSendingData(false);
		//clients[c].setSendingData(false);
			if(!c.isLoggedin()){
      send_data(sock,"LOGON: ");
	}else{
		    send_data(sock,"> ");
	}
	
}
}

void WarGServer::close_connection(int sock){

	    close(sock);
			      FD_CLR(sock,&master);
				for(int r=0;r<clients.size();r++){
					if(clients[r].sck()==sock){
						string lg="Connection from "+clients[r].getIp()+" lost";
					//	writeLog(lg);
			    		clients.erase(clients.begin()+r);
					}
				}
}

void WarGServer::writeLog(string log){
                     ofstream logfile;
                 	logfile.open("log.txt",ios_base::app);
                 	logfile<<log<<"\n";

}
