LDFLAGS:= -lSDL_bgi -lSDL2
CPPFLAGS:= -Wall -g -std=c++11

Debug: rivergen
Release: all

all: rivergen Makefile

rivergen: main.o src/water_map.o
	g++ -g -o $@ $^ $(LDFLAGS)

cleanDebug: clean

clean:
	rm *.o
	rm */*.o
