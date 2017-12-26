#include "../include/water_map.h"
#include <cstdio>
#include <cmath>

extern "C"{
#include <graphics.h>
}

water_map::water_map(int size_x, int size_y)
{
	water_map(size_x,size_y,1e10);
}


water_map::water_map(int size_x, int size_y,float max_height)
{
	//ctor
	if(size_x > WATER_MAP_MAX_SIZE || size_y > WATER_MAP_MAX_SIZE){
		throw 2;
	}
	m_size_x=size_x;
	m_size_y=size_y;
	unsigned int i, j;
	m_max_height=-1e10;
	m_min_height=1e10;
	for (i = 0; i < m_size_x; i++) {
		for (j = 0; j < m_size_y; j++) {
			float xx,yy;
			xx=(float)i/(float)m_size_x;
			yy=(float)j/(float)m_size_y;
			float k=(yy-.5)/4;
			m_map[i][j].land_height = (xx/4+20) / (xx+20) * (0.5+k*k);
			m_map[i][j].flags=0;
			m_map[i][j].curr_vx=m_map[i][j].curr_vy=m_map[i][j].water_height=0;
			if(m_map[i][j].land_height>max_height){
				m_map[i][j].land_height=max_height;
				m_map[i][j].flags&=PERMANENTLY_BORING;
			}
			if(m_map[i][j].land_height>m_max_height){
				m_max_height=m_map[i][j].land_height;
			}
			if(m_min_height>m_map[i][j].land_height){
				m_min_height=m_map[i][j].land_height;
			}
		}
	}
	j=(m_size_y*2)/3;
	while(j>0 && fabs(m_map[0][j].land_height-max_height)<1e-6){
		j--;
	}
	m_map[0][j].flags&=IS_WATER_SOURCE&HAS_WATER;
	m_map[0][j].water_height=m_map[0][j].land_height+1 CM;
	printf("%f %f\n",m_max_height,m_min_height);
}


void water_map::graph(){
	static int frame=0;
	if(frame==0){
		initwindow(m_size_x,m_size_y);
	}
	unsigned int i,j;
	for (i = 0; i < m_size_x; i++) {
		for (j = 0; j < m_size_y; j++) {
			if(m_map[i][j].flags&HAS_WATER){
				putpixel(i,j,BLUE);
			}else{
				putpixel(i,j,getMapColour(m_map[i][j].land_height,this));
			}
		}
	}
	char name[64];
	refresh();
	sprintf(name,"save_%d.bmp",frame++);
	writeimagefile(name,0,0,m_size_x,m_size_y);
}

int getMapColour(float height,water_map* w){
	float t;
	if(w->Get_max_height()-w->Get_min_height()==0){
		throw 1;
	}
	t=((height-w->Get_min_height())/(w->Get_max_height()-w->Get_min_height()));
	t*=100;
	//printf("%f %f\n",height,t);
	return COLOR((t>10)?t*2.5:255-t*25,255-2*t,100+1.55*t);
}

void water_map::step(){
	static double timestep=0.01;
	map_cell* row;
	unsigned int i,j;
	float vv,maxvv;
	for(i=0;i<m_size_x;i++){
		row=m_map[i];
		for(j=0;j<m_size_y;j++){
			if(row[j].flags&1){
				vv= row[j].curr_vx*row[j].curr_vx + row[j].curr_vy*row[j].curr_vy;
				if(vv>maxvv){
					maxvv=vv;
				}
				if(row[j].curr_vx>0){
					if(row[j].curr_vy>0){
						if(row[j+1].land_height > row[j].land_height + row[j].water_height){
							row[j].delta_vx=-2*row[j].curr_vx;
							row[j].delta_vy=0;
							row[j].delta_water_height= -(row[j].curr_vy*timestep);
						}
					}
				}
			}
		}
	}
}
