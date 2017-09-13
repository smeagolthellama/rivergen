#include "water_map.h"

water_map::water_map(int size_x, int size_y)
{
	//ctor
	int i, j;

	for (i = 0; i < size_x; i++) {
		for (j = 0; j < size_y; j++) {
			m_hardness_map[i][j] = (.1 / (float)i) * ((float)j / 4.0) * ((float)j / 4.0);///!< szep, volgy alaku fuggveny. kep is van: images/volgy.svg es images/volgy2.svg.
		}
	}

	m_size_x = size_x;
	m_size_y = size_y;
}
