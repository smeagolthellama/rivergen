#include "../include/water_map.h"
#include "../include/myerrors.h"
#include <cstdio>
#include <cmath>
#include <iostream>

extern "C" {
#include <graphics.h>
}

#define IN_WATER 2e-3
#define OPTIMUM_FRACTION 0.2
#define Z0 1 MM

double gravitationalConstant = 9.81;
double cachedCd[cacheSize];



water_map::water_map(int size_x, int size_y)
{
	water_map(size_x, size_y, 1e10);
}

double cell_size_si = 1 CM; //!< size in meters of each cell(cells are square)
double kin_viscosity_si = 1e-6;

water_map::water_map(int size_x, int size_y, double max_height)
{
	//ctor
	if ((size_x > WATER_MAP_MAX_SIZE) || (size_y > WATER_MAP_MAX_SIZE)) {
		throw CANT_MAKE_MAP;
	}

	/*	if(cachedCd[0]==0){
			int tmp;
			for(int i=0;i<cacheSize;i++){
				cachedCd[i]=(0.4*0.4)/((tmp=(log(((double)i/kin_viscosity_si
										)/Z0)-1))*tmp);
			}
		}*/
	m_size_x = size_x;
	m_size_y = size_y;
	unsigned int i, j;
	m_max_height = -1e10;
	m_min_height = 1e10;
	steps=0;
	totaltime=0;

	for (i = 0; i < m_size_x; i++) {
		for (j = 0; j < m_size_y; j++) {
			double xx, yy;
			xx = (double)i / (double)m_size_x;
			yy = (double)j / (double)m_size_y;
			double k = (yy - .5) / 4;
			m_map[i][j].land_height = ((xx / 4 + 15) / (xx + 15) * (0.5 + k * k))*2;
			m_map[i][j].flags = 0;
			m_map[i][j].curr_vx = m_map[i][j].curr_vy =  0;
			m_map[i][j].water_height = m_map[i][j].land_height;

			if (m_map[i][j].land_height > max_height) {
				m_map[i][j].land_height = max_height;
				m_map[i][j].flags &= PERMANENTLY_BORING;
			}

			if (m_map[i][j].land_height > m_max_height) {
				m_max_height = m_map[i][j].land_height;
			}

			if (m_min_height > m_map[i][j].land_height) {
				m_min_height = m_map[i][j].land_height;
			}
		}
	}

	j = (m_size_y * 4) / 5;
	
	while (j > 0 && fabs(m_map[0][j].land_height - max_height) < 1e-6) {
		j--;
	}
	for(i=j;i>0 && i>j-10;i--){
		m_map[j-i][i].flags |= IS_WATER_SOURCE | HAS_WATER;
		m_map[j-i][i].water_height = m_map[0][j].land_height + IN_WATER;
		printf("%f %f %u\n", m_max_height, m_min_height, i);
	}
	limiting_i=limiting_j=0;
}


void water_map::graph()
{
	static int frame = 0;

	if (frame == 0) {
		initwindow(m_size_x * 3, m_size_y);
	}

	unsigned int i, j;

	for (i = 0; i < m_size_x; i++) {
		for (j = 0; j < m_size_y; j++) {
			if (m_map[i][j].flags & (HAS_WATER | IS_WATER_SOURCE)) {
				if(m_map[i][j].flags & IS_WATER_SOURCE) {
					putpixel(i, j, RED);
					putpixel(i + m_size_x, j, RED);
					putpixel(i + 2 * m_size_x, j, RED);
				} else {
					putpixel(i, j, getHeightColour(m_map[i][j].water_height, this));
					putpixel(i + m_size_x, j, getWaterColour(m_map[i][j].water_height, this));
					putpixel(i + 2* m_size_x, j, getWaterDepthColour(m_map[i][j].water_height - m_map[i][j].land_height, this));
					//printf("%d %d %f\n",i,j,m_map[i][j].water_height-m_map[i][j].land_height);
				}
			} else {
				putpixel(i, j, getHeightColour(m_map[i][j].land_height, this));
				putpixel(i + m_size_x, j, getMapColour(m_map[i][j].land_height, this));
				putpixel(i + 2 * m_size_x, j, BROWN);
			}
		}
	}

#ifdef __linux
	refresh();
#endif
	char name[64];
	sprintf(name, "save_%d.bmp", frame++);
	writeimagefile(name, 0, 0, m_size_x * 3, m_size_y);
}

int getMapColour(double height, water_map *w)
{
	double t;

	if (w->Get_max_height() - w->Get_min_height() == 0) {
		throw NO_HEIGHT_DIFFERENCE;
	}

	t = ((height - w->Get_min_height()) / (w->Get_max_height() - w->Get_min_height()));
	t *= 100;
	//printf("%f %f\n",height,t);
	return COLOR((t > 10) ? 128 + t * 1.25 : 255 - t * 12, 255 - t, 175 + 0.75 * t);
}

int getHeightColour(double height, water_map *w)
{
	double t;

	if (w->Get_max_height() - w->Get_min_height() == 0) {
		throw NO_HEIGHT_DIFFERENCE;
	}

	t = ((height - w->Get_min_height()) / (w->Get_max_height() - w->Get_min_height()));
	t *= 100;
	//printf("%f %f\n",height,t);
	return COLOR(2.55 * t, 2.55 * t, 2.55* t);
}

int getWaterDepthColour(double height, water_map *w)
{
	double t;

	if (w->Get_max_height() - w->Get_min_height() == 0) {
		throw NO_HEIGHT_DIFFERENCE;
	}

	t = ((height) / (IN_WATER));
	t *= 100;
	//printf("%f %f\n",height,t);
	return COLOR((t > 10) ? t * 2.5 : 255 - t * 25, 255 - 2 * t, 100 + 1.55 * t);
}


int getWaterColour(double height, water_map *w)
{
	double t;

	if (w->Get_max_height() - w->Get_min_height() == 0) {
		throw NO_HEIGHT_DIFFERENCE;
	}

	t = ((height - w->Get_min_height()) / (w->Get_max_height() - w->Get_min_height()));
	t *= 100;
	//printf("%f %f\n",height,t);
	return COLOR((t > 10) ? t * 2.5 : 255 - t * 25, 255 - 2 * t, 100 + 1.55 * t);
}

inline int reynolds(int v, int h)
{
	return v * h / kin_viscosity_si;
}

double water_map::step()
{
	static double timestep = 0.01;
	static const signed char delta_is[8] = {-1,-1, 0, 1, 1, 1, 0,-1};
	static const signed char delta_js[8] = { 0, 1, 1, 1, 0,-1,-1,-1};
	static const double      massfrac[8] = { 1,.5, 1,.5, 1,.5, 1,.5};
	static const double velocityfrac[8] =  {1,.707,1,.707,1,.707,1,.707};
	map_cell *row;
	unsigned int i, j,ti,tj;
	double vv, maxvv, vx, vy;
	double delta_a, delta_b, delta_ab, delta_h;
	int   delta_i, delta_j;
	double volume, velocity;
	char flag;
	double min_tc = timestep * 1.1; //minty sea :D
	double cell_tc;//salty sea
	double othr_d_w=0;

	try {
		//zero all deltas
		//the deltas have allready been aplied
		steps++;
		maxvv=0;
		for(i = 0; i < m_size_x; i++) {
			for(j = 0; j < m_size_y; j++) {
				m_map[i][j].delta_vx = m_map[i][j].delta_vy = m_map[i][j].delta_water_height = 0;
			}
		}

		//calculate deltas
		for (i = 0; i < m_size_x; i++) {
			row = m_map[i];

			for (j = 0; j < m_size_y; j++) {
				if(steps==23 && 3==i && 165==j){
					printf(".");
				}
				if (row[j].flags & HAS_WATER) {
					flag = 0;
					vx = row[j].curr_vx;
					vy = row[j].curr_vy; //faster access to velocities
					vv = vx * vx + vy * vy; //velocity^2
					
					if (vv > maxvv) {
						maxvv = vv;
						ti=i;
						tj=j;
					}

					delta_ab = fabs(vx * timestep * vy * timestep) / (cell_size_si * cell_size_si);// mass fraction into diagonal neighbour
					delta_a = fabs(vx) * timestep / cell_size_si - delta_ab;//mass fraction that goes to cell a(x-neighbouring)
					delta_b = fabs(vy) * timestep / cell_size_si - delta_ab;//mass fraction into y-neighbour
					delta_j = (vx > 0) ? 1 : -1;
					delta_i = (vy > 0) ? 1 : -1;
					if(delta_ab>=1 || delta_a>=1 || delta_b>=1){
						std::cerr<<"excessive velocity: (ab,a,b,vx,vy)"<<delta_ab<<' '<<delta_a<<' '<<delta_b<<' '<<vx<<' '<<vy<<'\n';
						throw TIMESTEP_PANIC;
					}
					//momentum transfer
					double d_w = (m_map[i][j].water_height - m_map[i][j].land_height);
					if (d_w>1e-6) { // one micron of water will be held in place by surface tension!

						if(delta_a != 0) {
							if(j + delta_j > 0 && (j + delta_j) < m_size_y) {
								if (m_map[i][j].water_height < m_map[i][j + delta_j].land_height) {
									//can't go to x-neighbouring cell
									//in this case, water bounces back into the original cell
									m_map[i][j].delta_vx -= 2 * delta_a * vx;
									flag |= 8;
								} else {
									othr_d_w=m_map[i][j + delta_j].water_height - m_map[i][j + delta_j].land_height;
									m_map[i][j + delta_j].delta_vy += momentumtransfer(delta_a*d_w,vy,othr_d_w,m_map[i][j + delta_j].curr_vy) - m_map[i][j + delta_j].curr_vy;
									m_map[i][j + delta_j].delta_vx += momentumtransfer(delta_a*d_w,vx,othr_d_w,m_map[i][j + delta_j].curr_vx) - m_map[i][j + delta_j].curr_vx;
									m_map[i][j].delta_water_height -= delta_a * d_w;
									m_map[i][j + delta_j].delta_water_height += delta_a * d_w;
								}

								flag |= 1;
							}
						}

						if(delta_b != 0) {
							if(i + delta_i > 0 && i + delta_i < m_size_x ) {
								if( m_map[i][j].water_height < m_map[i + delta_i][j].land_height) {
									m_map[i][j].delta_vy -= 2 * delta_b * vy;
									flag |= 16;
								} else {
									othr_d_w=m_map[i + delta_i][j].water_height - m_map[i + delta_i][j].land_height;
									m_map[i + delta_i][j].delta_vx += momentumtransfer(delta_b*d_w,vx,othr_d_w,m_map[i + delta_i][j].curr_vx) - m_map[i + delta_i][j].curr_vx;
									m_map[i + delta_i][j].delta_vy += momentumtransfer(delta_b*d_w,vy,othr_d_w,m_map[i + delta_i][j].curr_vy) - m_map[i + delta_i][j].curr_vy;
									m_map[i][j].delta_water_height -= delta_b * d_w;
									m_map[i + delta_i][j].delta_water_height += delta_b * d_w;
								}
								flag |= 2;
							}
						}
						if((flag & 3) == 3) {
							if( m_map[i][j].water_height < m_map[i + delta_i][j + delta_j].land_height) {
								m_map[i][j].delta_vx -= 2 * delta_a * vx;
								m_map[i][j].delta_vy -= 2 * delta_b * vy;
								flag |= 32;
							} else {
								othr_d_w=m_map[i + delta_i][j + delta_j].water_height - m_map[i + delta_i][j + delta_j].land_height;
								m_map[i + delta_i][j + delta_j].delta_vx += momentumtransfer(delta_ab*d_w,vx,othr_d_w,m_map[i+delta_i][j+delta_j].curr_vx) - m_map[i+delta_i][j+delta_j].curr_vx;
								m_map[i + delta_i][j + delta_j].delta_vy += momentumtransfer(delta_ab*d_w,vy,othr_d_w,m_map[i+delta_i][j+delta_j].curr_vy) - m_map[i+delta_i][j+delta_j].curr_vy;
								m_map[i][j].delta_water_height -= delta_ab * d_w;
								m_map[i + delta_i][j + delta_j].delta_water_height += delta_ab * d_w;
							}
							flag |= 4;
						}
					}
					if(fabs(m_map[i][j].delta_vx)>1 || fabs(m_map[i][j].delta_vy)>1){
						std::cerr<<"Large accelaration (at momentum transfer) of "<<m_map[i][j].delta_vx<<' '<<m_map[i][j].delta_vy<<" at cycle "<<steps<<", i "<<i<<", j "<<j<<'.'<<std::endl;
						throw TIMESTEP_PANIC;
					}
					if(steps==23 && i==3 && j==163){
						printf(".");
					}
					//TO DO (mark#7#30/12/17): friction
					double h = m_map[i][j].water_height - m_map[i][j].land_height;
					double u = 2 * sqrt(vv);
					int RN = reynolds(u, h);

					if(RN > 2300) {
						//throw PROGRAMMING_PANIC;
						std::cerr << "Please tell my programmer that he should have implemented turbulent flow and transitional flow as well. Please tell him that reynolds number was the following: " << RN <<", velocity was "<<u<<" and height was "<<h<< std::endl;
					} else {
						double a = (u * kin_viscosity_si) / (h * h);
						if ((a*timestep)<1e-10) {
							a=0;
						}

						if(a != 0 && h != 0) {
							if(a * timestep * a * timestep > vv) {
								m_map[i][j].delta_vx = -m_map[i][j].curr_vx;
								m_map[i][j].delta_vy = -m_map[i][j].curr_vy;
							} else {
								m_map[i][j].delta_vx -= a * timestep * (m_map[i][j].curr_vx / (u * .5));
								m_map[i][j].delta_vy -= a * timestep * (m_map[i][j].curr_vy / (u * .5));
							}
						}
					}
					
					if(fabs(m_map[i][j].delta_vx)>1 || fabs(m_map[i][j].delta_vy)>1){
						std::cerr<<"Large accelaration (at friction braking) of "<<m_map[i][j].delta_vx<<' '<<m_map[i][j].delta_vy<<" at cycle "<<steps<<", i "<<i<<", j "<<j<<'.'<<std::endl;
						throw TIMESTEP_PANIC;
					}

					//spread out
					for (int k = 0; k < 8; k++) {
						delta_h = 0;

						if((i + delta_is[k] >= 0) && (j + delta_js[k]) >= 0 && (i + delta_is[k]) < m_size_x && (j + delta_js[k]) < m_size_y) {
							if(m_map[i + delta_is[k]][j + delta_js[k]].water_height < m_map[i][j].land_height) {
								delta_h = m_map[i][j].water_height - m_map[i][j].land_height;
							} else {
								delta_h = m_map[i][j].water_height - m_map[i + delta_is[k]][j + delta_js[k]].water_height;
							}
						}

						if(delta_h <= 0) {
							//no height diference.
							continue;
						}

						//calculate volume
						volume = 0.5 * massfrac[k] * gravitationalConstant * delta_h * delta_h * timestep * timestep;

						//if I am using more water than I have, PANIC!!!
						if(volume > (cell_size_si * cell_size_si * delta_h)) {
							std::cerr<<"Splurging in direction "<<k<<" (i+="<<delta_is[k]<<",j+="<<delta_js[k]<<")\tvolume: "<<volume<<", available: "<<delta_h*cell_size_si*cell_size_si<<"\n";
							throw TIMESTEP_PANIC;
						}

						//printf("%lg\n",volume);
						//add volume
						double d_wh = volume / (cell_size_si * cell_size_si);
						m_map[i + delta_is[k]][j + delta_js[k]].delta_water_height += d_wh;
						m_map[i][j].delta_water_height -= d_wh;
						//calculate velocity
						velocity = 2 * gravitationalConstant * delta_h * timestep / cell_size_si;

						//modify timestep
						if((cell_tc =( OPTIMUM_FRACTION * cell_size_si / velocity)) < min_tc) {
							min_tc = cell_tc;
							limiting_i = i;
							limiting_j = j;
						}

						//add velocity
						float mtx,mty;
						othr_d_w = m_map[i + delta_is[k]][j + delta_js[k]].water_height - m_map[i + delta_is[k]][j + delta_js[k]].land_height;
						mtx = momentumtransfer(volume,delta_js[k] *velocity*velocityfrac[k],othr_d_w*cell_size_si*cell_size_si,m_map[i + delta_is[k]][j + delta_js[k]].curr_vx)-m_map[i + delta_is[k]][j + delta_js[k]].curr_vx;
						mty = momentumtransfer(volume,delta_is[k] *velocity*velocityfrac[k],othr_d_w*cell_size_si*cell_size_si,m_map[i + delta_is[k]][j + delta_js[k]].curr_vy)-m_map[i + delta_is[k]][j + delta_js[k]].curr_vy;
						m_map[i + delta_is[k]][j + delta_js[k]].delta_vx +=  mtx;
						m_map[i + delta_is[k]][j + delta_js[k]].delta_vy +=  mty;
						
						if(fabs(m_map[i+delta_is[k]][j+delta_js[k]].delta_vx)>1 || fabs(m_map[i][j].delta_vy)>1){
							std::cerr<<"Large accelaration (splurge) of "<<m_map[i+delta_is[k]][j+delta_js[k]].delta_vx<<' '<<m_map[i+delta_is[k]][j+delta_js[k]].delta_vy<<" at cycle "<<steps<<", i "<<i+delta_is[k]<<", j "<<j+delta_js[k]<<". momentums transfered (x y): "<<mtx<<' '<<mty<<" height diference: "<<delta_h<<std::endl;
							throw TIMESTEP_PANIC;
						}
					}
				}
			}
		}
		
		if(min_tc*min_tc> OPTIMUM_FRACTION * OPTIMUM_FRACTION *cell_size_si*cell_size_si/maxvv){
			min_tc= OPTIMUM_FRACTION * cell_size_si/ sqrt(maxvv);
			limiting_i=ti;
			limiting_j=tj;
			std::cerr<<"seting min_tc to "<<min_tc<<", maxvv="<<maxvv<<"sqrt(maxvv)="<<sqrt(maxvv)<<'\n';
		}
		
		//TODO (mark#9#30/12/17): transfer mass(water +(eventually) eroded material)
		//aply deltas
		for(i = 0; i < m_size_x; i++) {
			for(j = 0; j < m_size_y; j++) {
				m_map[i][j].curr_vx += m_map[i][j].delta_vx;

				if(std::isnan(m_map[i][j].delta_vy)) {
					printf("nan found: i: %d; j: %d; timestep:%lf \n", i, j, timestep);
					throw PROGRAMMING_PANIC;
				}

				m_map[i][j].curr_vy += m_map[i][j].delta_vy;
				m_map[i][j].water_height += m_map[i][j].delta_water_height;

				//if(m_map[i][j].delta_water_height != 0) {
				//	printf("changing water on %d %d by %lf, to %lf\n", i, j, m_map[i][j].delta_water_height,m_map[i][j].water_height-m_map[i][j].land_height);
				//}

				/*
				m_map[i][j].land_height+=m_map[i][j].delta_land_height;
				*/
				if(m_map[i][j].delta_water_height > 1e-10) {
					if((m_map[i][j].flags & HAS_WATER) == 0) {
					//	printf("adding water to %d %d, %lf\n", i, j, m_map[i][j].delta_water_height);
						m_map[i][j].flags |= HAS_WATER;
					}
				}else{
					if(m_map[i][j].water_height <= m_map[i][j].land_height){
						m_map[i][j].flags&= (~HAS_WATER);
						m_map[i][j].water_height = m_map[i][j].land_height;
					}
				}

				if(m_map[i][j].flags & IS_WATER_SOURCE) {
					m_map[i][j].flags |= HAS_WATER;
					m_map[i][j].water_height = m_map[i][j].land_height + IN_WATER;
				}
				if(i==m_size_x-1){
					m_map[i][j].flags &= (~HAS_WATER);
					m_map[i][j].water_height=m_map[i][j].land_height;
				}
			}
		}
		totaltime+=timestep;
		timestep = min_tc;
		return timestep;
	} catch(int e) {
		if(e != TIMESTEP_PANIC) {
			throw;
		}
		
		std::cerr<<"timestep panic at i "<<i<<" j "<<j<<" timestep "<<timestep<<'\n';
		timestep /= 10;
		return -1;
	}
}

void water_map::status(){
	map_cell &c=m_map[limiting_i][limiting_j];
	std::cout<<steps<<'/'<<totaltime<< ':'<<limiting_i<<' '<<limiting_j<<": v(x y)"<<c.curr_vx<<' '<<c.curr_vy<<" water: "<<c.water_height<<" land: "<<c.land_height<<" deltas (same order): "<<c.delta_vx<<' '<<c.delta_vy<<' '<<c.delta_water_height<<std::endl;
}
