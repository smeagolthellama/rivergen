/**
 * a terkep, amin lesz a folyo.
 */

#ifndef WATER_MAP_H
#define WATER_MAP_H

//gondoltam, hasznaljuk a c++ altal adott tomb konyvtarat. nem biztos hogy jo otlet, de na.
//I thought to use the STL array, might not be good idea
#include <array>

///maximum size for map
///mekkora memoriat foglaljon le a program a terkep szamara?
#define WATER_MAP_MAX_SIZE 300

using std::array;

extern double cell_size_si; //!< size in meters of each cell(cells are square)
extern double density;
extern double gravitationalConstant;

#define cacheSize 100

extern double cachedCd[cacheSize];

enum flag_vals{
	HAS_WATER=1,
	IS_WATER_SOURCE=2,
	BORING=4,
	PERMANENTLY_BORING=8/*,
	CARRYING
	*/
};

struct map_cell{
	int flags; 
	double land_height;
	double curr_vx,curr_vy;
	double water_depth; ///not above sea level, but above land
	double delta_vx,delta_vy;
	double delta_water_height;
/** TODO (mark#1#12/26/17): add erosion */
	/*double delta_height;
	double carried;*/
};

#define KM *1000
#define M
#define CM *0.01
#define MM *0.001

#define momentumtransfer(m1,v1,m2,v2) (((m1)/((m1)+(m2)))*(2*v1-v2))

class water_map
{
	public:
		water_map(int size_x, int size_y);
		water_map(int size_x, int size_y,double max_height);
		//** Default destructor */
		//~water_map();
		double totaltime;
		int steps;
		unsigned int Get_size_x()
		{
			return m_size_x;
		};
		unsigned int Get_size_y()
		{
			return m_size_y;
		};
		double Get_max_height(){
			return m_max_height;
		}
		double Get_min_height(){
			return m_min_height;
		}
		void status();
		///maga a terkep.
		map_cell m_map[WATER_MAP_MAX_SIZE][WATER_MAP_MAX_SIZE]; //!< Member variable "m_hardness_map" describes map.

		void graph(); //!< function to draw map on screen

		double step(); //!< one iteration
	protected:
		///meretek
		unsigned int m_size_x; //!< Member variable "m_size_x", x size of map
		unsigned int m_size_y; //!< Member variable "m_size_y", y size of map
		double m_max_height; //!< maximum height of map
		double m_min_height; //!< minimum height of map
		int limiting_i,limiting_j; 
};

int getMapColour(double height,water_map* w);
int getHeightColour(double height,water_map* w);
int getWaterColour(double height, water_map *w);
int getWaterDepthColour(double height, water_map *w);

#endif // WATER_MAP_H
