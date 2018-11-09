#version 330 core

in vec4 frontColor;
out vec4 fragColor;

in vec2 vtexCoord;

uniform bool classic;
const float PI = 3.1415926535897932384626433832795;

void main()
{
	if(classic){
		if(distance(vtexCoord,vec2(0.5,0.5))>0.2)
			fragColor = vec4(1.0);
		else
			fragColor = vec4(1.0,0.0,0.0,1.0);
	}
	
	else{
		vec2 u = vtexCoord -  vec2(0.5,0.5);
		float theta = atan(u.t,u.s);
		if(distance(vtexCoord,vec2(0.5,0.5))<0.2 || mod(theta*16/PI+0.5,2)<1)
			fragColor = vec4(1.0,0.0,0.0,1.0);
		else 
			fragColor = vec4(1.0);
	
	}
 
}
