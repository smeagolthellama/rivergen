#version 3.7;
#include "textures.inc"
#include "colors.inc"

global_settings{assumed_gamma 1}

camera{
	angle		35
	location	<3.0,1.0,-2>
	look_at		<0,0,0>
	rotate		<0,-360*(clock),0>
}
light_source{
	<30,200,-150>,1
}
object{
	height_field{
		png concat("cropped_save_",str(floor(clock*2),0,0),"-0.png")
		smooth
	}
	texture{
		pigment{
			image_map{
				png concat("cropped_save_",str(floor(clock*2),0,0),"-2.png")
			}
		}
		rotate <90,0,0>
	}
	translate <-0.5,-0.5,-0.5>
}
