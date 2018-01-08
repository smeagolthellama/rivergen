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
#define WATER_MAP_MAX_SIZE 400

using std::array;

extern float cell_size_si; //!< size in meters of each cell(cells are square)
extern float density;
extern float gravitationalConstant;

#define cacheSize 100

extern float cachedCd[cacheSize];

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
	float land_height;
	float curr_vx,curr_vy;
	double water_height; ///above sea level, not above land
	double delta_vx,delta_vy;
	double delta_water_height;
/** TODO (mark#1#12/26/17): add erosion */
	float delta_height;
	float carried;
};

#define KM *1000
#define M
#define CM *0.01
#define MM *0.001



class water_map
{
	public:
		water_map(int size_x, int size_y);
		water_map(int size_x, int size_y,float max_height);
		//** Default destructor */
		//~water_map();
		unsigned int Get_size_x()
		{
			return m_size_x;
		};
		unsigned int Get_size_y()
		{
			return m_size_y;
		};
		float Get_max_height(){
			return m_max_height;
		}
		float Get_min_height(){
			return m_min_height;
		}
		///maga a terkep.
		map_cell m_map[WATER_MAP_MAX_SIZE][WATER_MAP_MAX_SIZE]; //!< Member variable "m_hardness_map" describes map.

		void graph(); //!< function to draw map on screen

		double step(); //!< one iteration
	protected:
		///meretek
		unsigned int m_size_x; //!< Member variable "m_size_x", x size of map
		unsigned int m_size_y; //!< Member variable "m_size_y", y size of map
		float m_max_height; //!< maximum height of map
		float m_min_height; //!< minimum height of map
};

int getMapColour(float height,water_map* w);
int getWaterColour(float height, water_map *w);

#endif // WATER_MAP_H
