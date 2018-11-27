#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;


uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform float time;

void main()
{
	float phi = -time*texCoord.s;
	mat3 rot = mat3(vec3(cos(phi),0,-sin(phi)),vec3(0,1,0),vec3(sin(phi),0,cos(phi)));
	vec3 position = rot*vertex;
    	gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}