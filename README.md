# rivergen
Simulate water-flow, ignoring surface tension, and make images out of the simulation. Implemented in c++.

# Dependencies

 - a BGI implementation. Only tested on sdl-bgi(http://libxbgi.sourceforge.net/) 
 - Python 2 if you want a diferent gradient for water on the rightmost map.
 - make

# Aditional tools:

To change the gradient, get a gimp gradient file (a .ggr), and replace  getGradient.py line 3 with `g = ggr.GimpGradient("PATH")` where PATH is where the .ggr is. Then, run `make` or `make gradient.txt`.

Something fun to do is to use VoxelSpace (https://github.com/s-macke/VoxelSpace) to explore the generated river/stream.
To do this, you will need to have it cloned to somewhere you can access, and to cut at least one frame into 3 and scale each part to 1024x1024. 
To do this with imagemagick, run 
    convert save_X.bmp -crop 250x250 -resize 1024x1024  croppedX.png
Where X is the frame you select. I recommend 250, 500, and 1000.
After this, copy the new PNGs into the maps folder in the VoxelSpace folder.
Next, you will need to open VoxelSpace.html in a browser that supports JavaScript, and has a console. Once it is open, open the console and type `LoadMap("croppedX-a;croppedbwX-0")` where X is the same as it was last time, and `a` is either 1 or 2, depending on which colouration you want. The gradient only affects 2. You might also want to try `a` as 0, to have the leftmost colouration.

