LDFLAGS:= -lSDL_bgi -lSDL2
CPPFLAGS:= -Wall -g -std=c++11
TIMESTAMP:= $(shell date +%Y%M%d-%H%m)

Debug: all 
Release: all

all: rivergen Makefile gradient.txt

gradient.txt: getGradient.py
	python $^ > $@
rivergen: main.o src/water_map.o
	g++ -g -o $@ $^ $(LDFLAGS)

cleanDebug: clean

veryclean: clean
	mkdir $(TIMESTAMP); mv *.bmp $(TIMESTAMP)
	

clean:
	-rm *.o
	-rm */*.o
