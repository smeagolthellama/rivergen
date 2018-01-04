LDFLAGS:= -lSDL_bgi -lSDL2
CPPFLAGS:= -Wall -g

all: rivergen Makefile

rivergen: main.o src/water_map.o
	g++ -g -o $@ $^ $(LDFLAGS)
