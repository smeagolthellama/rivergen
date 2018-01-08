#include "../include/water_map.h"
#include "../include/myerrors.h"
#include <cstdio>
#include <cmath>
#include <iostream>

extern "C" {
#include <graphics.h>
}

#define OPTIMUM_FRACTION 0.2
#define Z0 1 MM

float gravitationalConstant=9.89;
float cachedCd[cacheSize];



water_map::water_map(int size_x, int size_y)
{
	water_map(size_x, size_y, 1e10);
}

float cell_size_si=1 CM; //!< size in meters of each cell(cells are square)
float kin_viscosity_si=1e-6;

water_map::water_map(int size_x, int size_y, float max_height)
{
	//ctor
	if ((size_x > WATER_MAP_MAX_SIZE) || (size_y > WATER_MAP_MAX_SIZE)) {
		throw CANT_MAKE_MAP;
	}
/*	if(cachedCd[0]==0){
		int tmp;
		for(int i=0;i<cacheSize;i++){
			cachedCd[i]=(0.4*0.4)/((tmp=(log(((float)i/kin_viscosity_si
									)/Z0)-1))*tmp);
		}
	}*/
	m_size_x = size_x;
	m_size_y = size_y;
	unsigned int i, j;
	m_max_height = -1e10;
	m_min_height = 1e10;

	for (i = 0; i < m_size_x; i++) {
		for (j = 0; j < m_size_y; j++) {
			float xx, yy;
			xx = (float)i / (float)m_size_x;
			yy = (float)j / (float)m_size_y;
			float k = (yy - .5) / 4;
			m_map[i][j].land_height = (xx / 4 + 20) / (xx + 20) * (0.5 + k * k);
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

	j = (m_size_y * 2) / 3;

	while (j > 0 && fabs(m_map[0][j].land_height - max_height) < 1e-6) {
		j--;
	}

	m_map[0][j].flags |= IS_WATER_SOURCE | HAS_WATER;
	m_map[0][j].water_height = m_map[0][j].land_height + 1 MM;
	printf("%f %f %u\n", m_max_height, m_min_height,j);
}


void water_map::graph()
{
	static int frame = 0;

	if (frame == 0) {
		initwindow(m_size_x*3, m_size_y);
	}

	unsigned int i, j;

	for (i = 0; i < m_size_x; i++) {
		for (j = 0; j < m_size_y; j++) {
			if (m_map[i][j].flags & (HAS_WATER|IS_WATER_SOURCE)) {
				if(m_map[i][j].flags&IS_WATER_SOURCE){
					putpixel(i,j,RED);
					putpixel(i+m_size_x,j,RED);
					putpixel(i+2*m_size_x,j,RED);
				}else{
					putpixel(i, j, getWaterColour(m_map[i][j].water_height-m_map[i][j].land_height,this));
					putpixel(i+m_size_x, j, getWaterColour(m_map[i][j].water_height,this));
					putpixel(i+2*m_size_x, j, BLUE);
					//printf("%d %d %f\n",i,j,m_map[i][j].water_height-m_map[i][j].land_height);
				}

			} else {
				putpixel(i, j, getMapColour(m_map[i][j].land_height, this));
				putpixel(i+m_size_x, j, getMapColour(m_map[i][j].land_height, this));
				putpixel(i+2*m_size_x, j, BROWN);
			}
		}
	}
#ifdef __linux
	refresh();
#endif
	char name[64];

	sprintf(name, "save_%d.bmp", frame++);
	writeimagefile(name, 0, 0, m_size_x*3, m_size_y);

}

int getMapColour(float height, water_map *w)
{
	float t;

	if (w->Get_max_height() - w->Get_min_height() == 0) {
		throw NO_HEIGHT_DIFFERENCE;
	}

	t = ((height - w->Get_min_height()) / (w->Get_max_height()-w->Get_min_height()));
	t *= 100;
	//printf("%f %f\n",height,t);
	return COLOR((t > 10) ? 128+ t * 1.25 : 255 - t * 12, 255 - t, 175 + 0.75 * t);
}

int getWaterColour(float height, water_map *w)
{
	float t;

	if (w->Get_max_height() - w->Get_min_height() == 0) {
		throw NO_HEIGHT_DIFFERENCE;
	}

	t = ((height - w->Get_min_height()) / (w->Get_max_height()-w->Get_min_height()));
	t *= 100;
	//printf("%f %f\n",height,t);
	return COLOR((t > 10) ? t * 2.5 : 255 - t * 25, 255 - 2 * t, 100 + 1.55 * t);
}

inline int reynolds(int v,int h){
	return v*h/kin_viscosity_si;
}

double water_map::step()
{
	static double timestep = 0.01;
	static const signed char delta_is[8] = {-1,-1, 0, 1, 1, 1, 0,-1};
	static const signed char delta_js[8] = { 0, 1, 1, 1, 0,-1,-1,-1};
	static const float       massfrac[8] = { 1,.5, 1,.5, 1,.5, 1,.5};
	map_cell *row;
	unsigned int i, j;
	float vv, maxvv, vx, vy;
	float delta_a, delta_b, delta_ab,delta_h;
	int   delta_i, delta_j;
	double volume, velocity;
	char flag;
	float min_tc=timestep*1.1;//minty sea :D
	float cell_tc;//salty sea
	try{
		//zero all deltas
		//the deltas have allready been aplied
		for(i=0; i<m_size_x; i++) {
			for(j=0; j<m_size_y; j++) {
				m_map[i][j].delta_vx=m_map[i][j].delta_vy=m_map[i][j].delta_water_height=0;
			}
		}
		//calculate deltas
		for (i = 0; i < m_size_x; i++) {
			row = m_map[i];

			for (j = 0; j < m_size_y; j++) {
				if (row[j].flags & HAS_WATER) {
					flag=0;
					vx = row[j].curr_vx;
					vy = row[j].curr_vy; //faster access to velocities
					vv = vx * vx + vy * vy; //velocity^2

					if (vv > maxvv) {
						maxvv = vv;
					}

					delta_ab = (vx * timestep * vy * timestep) / (cell_size_si * cell_size_si);
					delta_a = vx * timestep / cell_size_si - delta_ab;
					delta_b = vy * timestep / cell_size_si - delta_ab;
					delta_j = (vx > 0) ? 1 : -1;
					delta_i = (vy > 0) ? 1 : -1;
					//momentum transfer
					float d_w=(m_map[i][j].water_height-m_map[i][j].land_height);
					if(j+delta_j>0 && (j+delta_j)<m_size_y) {
						if (m_map[i][j].water_height<m_map[i][j+delta_j].land_height) {
						//can't go to x-neighbouring cell
						//in this case, water bounces back into the original cell
							m_map[i][j].delta_vx-= 2*delta_a*vx;
							flag|=8;
						}else{
							m_map[i][j+delta_j].delta_vy+=(delta_j*(delta_a*d_w*vy)+(m_map[i][j+delta_j].water_height-m_map[i][j+delta_j].land_height)*m_map[i][j+delta_j].curr_vy)/(delta_a*d_w+(m_map[i][j+delta_j].water_height-m_map[i][j+delta_j].land_height));
							m_map[i][j].delta_water_height-=delta_a*d_w;
						}
						flag|=1;
					}
					if(i+delta_i>0 && i+delta_i<m_size_x ) {
						if( m_map[i][j].water_height<m_map[i+delta_i][j].land_height){
							m_map[i][j].delta_vy-= 2*delta_b*vy;
							flag|=16;
						}else{
						
							m_map[i+delta_i][j].delta_vx+=(delta_i*(delta_b*d_w*vx)+(m_map[i+delta_i][j].water_height-m_map[i+delta_i][j].land_height)*m_map[i+delta_i][j].curr_vx)/(delta_b*d_w+(m_map[i+delta_i][j].water_height-m_map[i+delta_i][j].land_height));
							m_map[i][j].delta_water_height-=delta_b*d_w;
						}
						flag|=2;
					}
					if((flag&3)==3){
						if( m_map[i][j].water_height<m_map[i+delta_i][j+delta_j].land_height) {
							m_map[i][j].delta_vx-= 2*delta_a*vx;
							m_map[i][j].delta_vy-= 2*delta_b*vy;
							flag|=32;
						}else{
							m_map[i+delta_i][j+delta_j].delta_vx+=(delta_j*(delta_a*d_w*vy)+(m_map[i+delta_i][j+delta_j].water_height-m_map[i+delta_i][j+delta_j].land_height)*m_map[i+delta_i][j+delta_j].curr_vy)/(delta_a*d_w+(m_map[i+delta_i][j+delta_j].water_height-m_map[i+delta_i][j+delta_j].land_height));						
							m_map[i+delta_i][j+delta_j].delta_vy+=(delta_i*(delta_b*d_w*vx)+(m_map[i+delta_i][j+delta_j].water_height-m_map[i+delta_i][j+delta_j].land_height)*m_map[i+delta_i][j+delta_j].curr_vx)/(delta_b*d_w+(m_map[i+delta_i][j+delta_j].water_height-m_map[i+delta_i][j+delta_j].land_height));
							m_map[i][j].delta_water_height-=delta_ab*d_w;
						}
						flag|=4;
					}
					if(std::isnan(m_map[i][j].delta_water_height)){
						printf("nan found: d_a: %f; d_b: %f; d_ab: %f; d_w:%f; cellsize:%f;timestep:%f \n",delta_a,delta_b,delta_ab,d_w,cell_size_si,timestep);
						throw PROGRAMMING_PANIC;
					}
					//TO DO (mark#7#30/12/17): friction
					float h=m_map[i][j].water_height-m_map[i][j].land_height;
					float u=2*sqrt(vv);
					int RN=reynolds(u,h);
					if(RN>2300){
						//throw PROGRAMMING_PANIC;
						std::cerr<<"please tell my programmer that he should have implemented turbulent flow and transitional flow as well. Please tell him that reynolds number was the following: "<<RN<<std::endl;
					}else{
						float a=(u*kin_viscosity_si)/(h*h);
						if(m_map[i][j].curr_vx>0){
							m_map[i][j].delta_vx-=a*timestep*(m_map[i][j].curr_vx/(m_map[i][j].curr_vx+m_map[i][j].curr_vy));
						}else{
							m_map[i][j].delta_vx+=a*timestep*(m_map[i][j].curr_vx/(m_map[i][j].curr_vx+m_map[i][j].curr_vy));
						}
						if(m_map[i][j].curr_vx>0){
							m_map[i][j].delta_vy-=a*timestep*(m_map[i][j].curr_vy/(m_map[i][j].curr_vx+m_map[i][j].curr_vy));
						}else{
							m_map[i][j].delta_vy+=a*timestep*(m_map[i][j].curr_vy/(m_map[i][j].curr_vx+m_map[i][j].curr_vy));
						}
					}
					//spread out
					for (int k = 0; k < 8; k++) {
						delta_h=0;
						if((i+delta_is[k]>=0) && (j+delta_js[k])>=0 && (i+delta_is[k])<m_size_x && (j+delta_js[k])<m_size_y){
							if(m_map[i+delta_is[k]][j+delta_js[k]].water_height<m_map[i][j].land_height) {
								delta_h=m_map[i][j].water_height-m_map[i][j].land_height;
							} else {
								delta_h=m_map[i][j].water_height-m_map[i+delta_is[k]][j+delta_js[k]].water_height;
							}
						}
						if(delta_h<=0) {
							//no height diference.
							continue;
						}
						//calculate volume
						volume = 0.5*massfrac[k]*gravitationalConstant*delta_h*delta_h*timestep*timestep;
						//if I am using more water than I have, PANIC!!!
						if(volume>(cell_size_si*cell_size_si*delta_h)){
							throw TIMESTEP_PANIC;
						}
						//printf("%lg\n",volume);
						//add volume
						float d_wh=volume/(cell_size_si*cell_size_si);
						m_map[i+delta_is[k]][j+delta_js[k]].delta_water_height+=d_wh;
						m_map[i][j].delta_water_height-=d_wh;
						//calculate velocity
						velocity = 2*gravitationalConstant*delta_h*timestep/cell_size_si;
						//modify timestep
						if((cell_tc=OPTIMUM_FRACTION*cell_size_si/velocity)<min_tc){
							min_tc=cell_tc;
						}
						
						//add velocity
						m_map[i+delta_is[k]][j+delta_js[k]].delta_vx+=delta_js[k]*(volume*velocity + (m_map[i+delta_is[k]][j+delta_js[k]].water_height-m_map[i+delta_is[k]][j+delta_js[k]].land_height)*cell_size_si*cell_size_si*m_map[i+delta_is[k]][j+delta_js[k]].curr_vx);
						m_map[i+delta_is[k]][j+delta_js[k]].delta_vy+=delta_is[k]*(volume*velocity + (m_map[i+delta_is[k]][j+delta_js[k]].water_height-m_map[i+delta_is[k]][j+delta_js[k]].land_height)*cell_size_si*cell_size_si*m_map[i+delta_is[k]][j+delta_js[k]].curr_vy);
					}
				}
			}
		}
		//TODO (mark#9#30/12/17): transfer mass(water +(eventually) eroded material)
		//aply deltas
		for(i=0;i<m_size_x;i++){
			for(j=0;j<m_size_y;j++){
				m_map[i][j].curr_vx+=m_map[i][j].delta_vx;
				m_map[i][j].curr_vy+=m_map[i][j].delta_vy;
				m_map[i][j].water_height+=m_map[i][j].delta_water_height;
				if(m_map[i][j].delta_water_height!=0){
					printf("adding extra water to %d %d, %f\n",i,j,m_map[i][j].delta_water_height);
				}
				/*
				m_map[i][j].land_height+=m_map[i][j].delta_land_height;
				*/
				if(m_map[i][j].delta_water_height>0 && (m_map[i][j].flags&HAS_WATER)==0){
					printf("adding water to %d %d, %f\n",i,j,m_map[i][j].delta_water_height);
					m_map[i][j].flags|=HAS_WATER;
				}
				if(m_map[i][j].flags&IS_WATER_SOURCE){
					m_map[i][j].flags|=HAS_WATER;
					m_map[i][j].water_height = m_map[i][j].land_height + 1 MM;
				}
			}
		}
		timestep=min_tc;
		return timestep;
	}catch(int e){
		if(e!=TIMESTEP_PANIC){
			throw;
		}
		timestep/=10;
		return -1;
	}
}
