#version 330 core

out vec4 fragColor;

in vec2 vtexCoord;

uniform int pause=2;
uniform float time;
uniform sampler2D eye0;
uniform sampler2D iris1;

void main()
{	
	float slice = 1.0/4;

	int frame = int(mod(time/slice,6));
	if(frame==4) frame=2;
	if(frame==5) frame=1;
	int moderation = int(mod(time/(6*slice*(pause-1)),2));
	int finalFrame = frame*moderation;

	vec4 eyeSprites = texture(eye0,vec2((vtexCoord.s+mod(finalFrame,4))/4,vtexCoord.t));

	if(eyeSprites.a <= 0.5)
		fragColor = texture(iris1,vtexCoord);
	else 
		fragColor = eyeSprites;

}
