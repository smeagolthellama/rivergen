#version 3.7;
#include "textures.inc"
#include "colors.inc"

global_settings{assumed_gamma 1}

camera{
	angle		35
	location	<4.0,2.0,-3>
	look_at		<0,0,0>
	rotate		<0,-360*(clock/10),0>
}
  sky_sphere {
    pigment {
      gradient y
      color_map {
        [ 0.5  color CornflowerBlue ]
        [ 1.0  color MidnightBlue ]
      }
      scale 2
      translate -1
    }
  }
light_source{
	<30,200,-150>,1
}
object{
	height_field{
		png concat("cropped_save_",str(floor(clock*10),0,0),"-0.png")
		smooth
	}
	texture{
		pigment{
			image_map{
				png concat("cropped_save_",str(floor(clock*10),0,0),"-2.png")
			}
		}
		rotate <90,0,0>
	}
	finish {
        	ambient .1
        	diffuse .1
        	specular 1
        	roughness .001
        	metallic
        	reflection {
        	  .75
        	  metallic
        	}
	}	
	translate <-0.5,-0.5,-0.5>
	scale <2.5,0.8,2.5>
}
