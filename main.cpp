#include <iostream>
#include <string.h>
#include <cstdlib>
#include "include/water_map.h"

#ifdef __linux
extern "C"{
#endif

#include <graphics.h>

#ifdef __linux
}
#endif

using std::cout;
using std::endl;

int main()
{
	water_map w(250,250,0.6);
	w.graph();
	getch();
	int i;
	float t;
	for(i=0;i<100000;i++){
		t=w.step();
		if(i%100==0){
			w.graph();
			cout<<t<<endl;
		}
	}
	getch();
	return 0;
}
