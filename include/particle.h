/**
 * A vizreszecske, ami tomegesen alkotja a folyot.
 */

#ifndef PARTICLE_H
#define PARTICLE_H

///Ha nem grafikat hasznalunk, mi legyen a partvonal ami meg nem volt hordva a folyo altal. Valoszinu nem hasznaljuk.
#define SHORE '"'
///Ha nem hasznalunk grafikat, mi legyen a folyo artere. Valoszinu nem hasznaljuk
#define BED   '`'
///ha nem hasznalunk grafikat, mi legyae a folyo. Valoszinu nem hasznaljuk
#define RIVER '#'

///milyen nagy legyen a "kep". valoszinu nem lesz hasznalva.
#define SIZE_Y 100
#define SIZE_X 100

//kell a terkep.
#include "water_map.h"


///Particle of water.
class particle
{
	public:
		/**constructor*/
		particle(water_map* w);
		/** Access m_position_x
		 * \return The current value of m_position_x
		 */
		unsigned int Getposition_x() { return m_position_x; }
		/*/** Set m_position_x
		 * \param val New value to set
		 */
		//void Setposition_x(unsigned int val) { m_position_x = val; }
		/** Access m_position_y
		 * \return The current value of m_position_y
		 */
		unsigned int Getposition_y() { return m_position_y; }
		/*/** Set m_position_y
		 * \param val New value to set
		 */
		//void Setposition_y(unsigned int val) { m_position_y = val; }
		/** Access m_speed_y
		 * \return The current value of m_speed_y
		 */
		 int Getspeed_y() { return m_speed_y; }
		/*/** Set m_speed_y
		 * \param val New value to set
		 */
		//void Setspeed_y( int val) { m_speed_y = val; }
		/** Access m_speed_x
		 * \return The current value of m_speed_x
		 */
		 int Getspeed_x() { return m_speed_x; }
		/*/** Set m_speed_x
		 * \param val New value to set
		 */
		//void Setspeed_x( int val) { m_speed_x = val; }
		/** Access m_carried
		 * \return The current value of m_carried
		 */
		unsigned int Getcarried() { return m_carried; }
		/*/** Set m_carried
		 * \param val New value to set
		 */
	protected:

	private:
		unsigned int m_position_x; //!< Member variable "m_position_x"
		unsigned int m_position_y; //!< Member variable "m_position_y"
		 int m_speed_y; //!< Member variable "m_speed_y"
		 int m_speed_x; //!< Member variable "m_speed_x"
		unsigned int m_carried; //!< Member variable "m_carried"
		water_map* m_water_map_pointer;
};

bool checkForCollision(particle);

#endif // PARTICLE_H
