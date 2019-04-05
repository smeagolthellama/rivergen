LDFLAGS:= -lSDL_bgi -lSDL2
CXXFLAGS:= -Wall -g -std=c++11
TIMESTAMP:= $(shell date +%Y%M%d-%H%m)
CXX=clang++

cropped: save_1000.bmp
	for a in save*.bmp; do convert $$a -crop 350x350 cropped_$$(basename -s .bmp $$a).png; done

all: rivergen Makefile gradient.txt presentation.pdf

Debug: all 
Release: all

gradient.txt: getGradient.py
	python $^ > $@
rivergen: main.o src/water_map.o
	$(CXX) -g -o $@ $^ $(LDFLAGS)

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

%.bmp: rivergen
	./rivergen

animate: animate.ini animate.pov cropped
	povray animate.ini
	
