LDFLAGS:= -lSDL_bgi -lSDL2
CPPFLAGS:= -Wall -g -std=c++11
TIMESTAMP:= $(shell date +%Y%M%d-%H%m)


all: rivergen Makefile gradient.txt presentation.pdf

Debug: all 
Release: all

gradient.txt: getGradient.py
	python $^ > $@
rivergen: main.o src/water_map.o
	g++ -g -o $@ $^ $(LDFLAGS)

cleanDebug: clean

veryclean: clean
	mkdir $(TIMESTAMP); mv *.bmp $(TIMESTAMP)
	
presentation.pdf: save_0.pdf presentation.tex movingcell.tex spill.tex spilldist.tex
	xelatex presentation.tex

clean:
	-rm *.o
	-rm */*.o

%.pdf: %.bmp
	convert $*.bmp $*.pdf

