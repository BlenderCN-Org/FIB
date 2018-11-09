#version 330 core

in vec4 frontColor;
out vec4 fragColor;

in vec2 vtexCoord;

void main()
{
	if(distance(vtexCoord,vec2(0.5,0.5))>0.2)
		fragColor = vec4(1.0);
	else
		fragColor = vec4(1.0,0.0,0.0,1.0);
}
