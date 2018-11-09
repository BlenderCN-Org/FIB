#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

out vec3 N;
//out vec3 P;
out vec3 L;
out vec3 V;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix, modelViewMatrixInverse;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;

uniform vec4 lightPosition;

uniform bool world;

void main()
{
	if(world){
		N = normal;
		vec3 P = vertex;
		L = (modelViewMatrixInverse*lightPosition).xyz - P;
		V = modelViewMatrixInverse[3].xyz - P;
	}
	else{
		N = normalMatrix * normal;
		vec3 P = (modelViewMatrix * vec4(vertex,1.0)).xyz;
		V = -P;
		L = (lightPosition).xyz - P;
	}
		
	vtexCoord = texCoord;
	gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
} 
