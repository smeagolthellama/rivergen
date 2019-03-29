#include "textures.inc"
#include "colors.inc"
camera{
	angle		35
	location	<3.0,1.0,-2>
	look_at		<-0.5,0.5,0.5>
	rotate		<0,-360*(clock),0>
}
light_source{
	<30,200,-150>,1
}
height_field{
	png concat("cropped_save_",str(floor(clock),0,0),"-0.png")
	smooth
	texture{
		pigment{
			image_map{
				png concat("cropped_save_",str(floor(clock),0,0),"-2.png")
			}
		}
	}
}
