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
		array<array<int, WATER_MAP_MAX_SIZE>, WATER_MAP_MAX_SIZE> m_hardness_map; //!< Member variable "m_hardness_map"
	protected:
		///meretek
		unsigned int m_size_x; //!< Member variable "m_size_x"
		unsigned int m_size_y; //!< Member variable "m_size_y"
};

#endif // WATER_MAP_H
