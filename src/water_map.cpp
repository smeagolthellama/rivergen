#include "../include/water_map.h"

extern "C"{
#include <graphics.h>
}

water_map::water_map(int size_x, int size_y)
{
	//ctor
	int i, j;
	m_map: arr
	for (i = 0; i < m_size_x; i++) {
		for (j = 0; j < m_size_y; j++) {
			float k=(float)j-(float)m_size_y/8;
			m_map[i][j].height = (.1 / ((float)i+1)) * k*k;///!< szep, volgy alaku fuggveny. kep is van: images/volgy.svg es images/volgy2.svg.
		}
	}

	m_size_x = size_x;
	m_size_y = size_y;
}


void water_map::graph(){
	int i,j;
	for (i = 0; i < m_size_x; i++) {
		for (j = 0; j < m_size_y; j++) {
			putpixel(i,j,COLOR(,,))
		}
	}
}
