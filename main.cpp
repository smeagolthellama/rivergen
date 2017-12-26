#include <iostream>
#include <string.h>
#include <cstdlib>
#include "include/particle.h"
#include "include/random.hpp"
#include "include/water_map.h"
extern "C"{
#include <graphics.h>
}
using std::cout;
using std::endl;

int main()
{
	water_map w(500,500,0.6);
	w.graph();
	getch();
	return 0;
}
