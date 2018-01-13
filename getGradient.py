import ggr

g = ggr.GimpGradient("/usr/share/gimp/2.0/gradients/Deep_Sea.ggr")

def getred(pos):
	return g.color(pos)[0];
def getgreen(pos):
	return g.color(pos)[1];
def getblue(pos):
	return g.color(pos)[2];

a=1;
while a>=0:
	print getred(a),' ',getgreen(a),' ',getblue(a)
	a-=0.001

