/**
 * a terkep, amin lesz a folyo.
 */

#ifndef WATER_MAP_H
#define WATER_MAP_H

//gondoltam, hasznaljuk a c++ altal adott tomb konyvtarat. nem biztos hogy jo otlet, de na.
#include <array>

///mekkora memoriat foglaljon le a program a terkep szamara?
#define WATER_MAP_MAX_SIZE 1024

using std::array;

struct map_cell{
	int flags; //!< &1=has water, &2=is carrying stuff, &4=
	float height;
};

class water_map
{
	public:
		/** Default constructor */
		water_map(int size_x, int size_y);
		/** Default destructor */
		~water_map();
		unsigned int Get_size_x()
		{
			return m_size_x;
		};
		unsigned int Get_size_y()
		{
			return m_size_y;
		};
		///milyen eros/kemeny a part.
		array<array<map_cell, WATER_MAP_MAX_SIZE>, WATER_MAP_MAX_SIZE> m_map; //!< Member variable "m_hardness_map" describes

		void graph(); //!< function to draw map on screen
	protected:
		///meretek
		unsigned int m_size_x; //!< Member variable "m_size_x", x size of map
		unsigned int m_size_y; //!< Member variable "m_size_y", y size of map
		float m_max_height; //!< maximum height of map
		float m_min_height; //!< minimum height of map
};

#endif // WATER_MAP_H
