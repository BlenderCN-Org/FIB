#version 330 core

in vec4 frontColor;
out vec4 fragColor;

in vec2 vtexCoord;
uniform float time;
uniform sampler2D sampler;

void main()
{
	if(fract(time)<=0.5)
		fragColor = texture(sampler,vtexCoord);
	else{
		if(distance(vtexCoord,vec2(0.393,0.652))<0.025)
			fragColor = texture(sampler,vec2(vtexCoord.s+0.057,vtexCoord.t-0.172));
		else
			fragColor = texture(sampler,vtexCoord);	
	}


}
