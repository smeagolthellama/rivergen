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
	water_map w(250,250,6);
	try{
		w.graph();
	}catch(int e){
		cerr<<e;
		throw;
	}
	getch();
	
	int i,panics=0;
	for(i=0;i<100000;i++){
		t=w.step();
		if(i%100==0){
			w.graph();
//		}
	//	if(i%10==0){
			w.status();
			cout<<"timestep: "<<t<<endl;
		}
		if(t==-1){
			cerr<<"timestep panic "<<panics<<" thrown, might be good idea to stop.\n";
			panics++;
			if(panics>100){
				return 1;
			}
		}
		
	}
	return 0;
}
