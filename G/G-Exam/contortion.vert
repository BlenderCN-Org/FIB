#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 2) in vec3 color;

out vec4 frontColor;

uniform mat4 modelViewProjectionMatrix;

uniform float time;

void main()
{	
	float A = 0;
	if(vertex.y >0.5) A = (vertex.y-0.5)*sin(time);
	mat3 rot = mat3(vec3(1,0,0),vec3(0,cos(A),sin(A)),vec3(0,-sin(A),cos(A)));
	mat4 translation = mat4(vec4(1,0,0,0),vec4(0,1,0,0),vec4(0,0,1,0),vec4(0,-1,0,1));

	vec3 pos = rot*(translation*vec4(vertex,1.0)).xyz;
	pos = (inverse(translation)*vec4(pos,1.0)).xyz;

    	frontColor = vec4(color,1.0);
    	gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
}
