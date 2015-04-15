#include "WarGServer.h"
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>
#define DEBUG
using namespace std;
int main(){
WarGServer warg;
#ifndef DEBUG
pid_t id=fork();
if(id==0){
warg.start();
}
#else
warg.start();
#endif
	return 0;
}
