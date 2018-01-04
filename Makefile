LDFLAGS:= -lSDL_bgi -lSDL2
CPPFLAGS:= -Wall -g
CCFLAGS:= -std=c++11

all: rivergen Makefile

rivergen: main.o src/water_map.o
	g++ -g -o $@ $^ $(LDFLAGS)
