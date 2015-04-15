#ifndef __CLIENTS_H__
#define __CLIENTS_H__
#include <string>
class Client{
bool loggedin;
int socket;
bool sending_data;
std::string ip_address;

public:
Client(){
	loggedin=false;
	sending_data=false;
}
Client(int sock){
	loggedin=false;
	socket=sock;
	sending_data=false;
}


void setLoggedin(bool state){
	loggedin=state;
}
bool isLoggedin(){return loggedin;}
int sck(){return socket;}
bool setSendingData(bool sd){
sending_data=sd;

}
bool isSendingData(){return sending_data;}



std::string getIp(){return ip_address;}
void setIp(std::string ipaddr){ ip_address=ipaddr;}
};



#endif 
