# rivergen
This is hopefully a physically accurate (I have my doubts) simulation of the flow of water, although it ignores surface tension. It is implemented in c++.

# Dependencies

 - c++
 - a BGI implementation. Only tested on [sdl-bgi](http://libxbgi.sourceforge.net/) 
    - note: winbgim doesn't work, unless writeimagefile, COLOR, and putpixel are defined. For some reason, the version that I was using had them prototyped, but not implemented.
 - Python 2 if you want a diferent gradient for water on the rightmost map.
 - make
 - Pov-ray and ImageMagick if you want the animation

# Aditional tools:

To change the gradient, get a gimp gradient file (a .ggr), and replace  getGradient.py line 3 with `g = ggr.GimpGradient("PATH")` where PATH is where the .ggr is. Then, run `make` or `make gradient.txt`.

Now included is an animation of a complete 100000 step run of the program (results shouldn't vary too much, although I haven't managed to reproduce *any* results on Windows). 
