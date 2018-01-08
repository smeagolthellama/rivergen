#include <iostream>
#include <string.h>
#include <cstdlib>
#include <csignal>
#include "include/water_map.h"

#ifdef __linux
extern "C"{
#endif

#include <graphics.h>

#ifdef __linux
}
#endif

using std::cout;
using std::cerr;
using std::endl;

double t;
void react_to_signals(int signum){
	switch(signum){
		case 10/*usr1*/:
			cerr<<"To kill this program, use sigint(ctrl-c). To output the current timestep to stderr (eg: if you are piping to a file), use sigusr2. To output this help, use sigusr1."<<endl;
			break;
		case 12:
			cerr<<t<<endl;
			break;
		case 2:
			exit(0);
			break;
		default:
			cerr<<"This... should not be happening. Please tell my developer.";
			exit(signum);
	}
}

int main()
{
	signal(SIGINT,react_to_signals);
	signal(SIGUSR1,react_to_signals);
	signal(SIGUSR2,react_to_signals);
	signal(SIGSEGV,react_to_signals);
	water_map w(250,250,0.6);
	w.graph();
	getch();
	int i;
	for(i=0;i<100000;i++){
		t=w.step();
		if(i%100==0){
			w.graph();
		}
		cout<<"timestep: "<<t<<endl;
	}
	getch();
	return 0;
}
