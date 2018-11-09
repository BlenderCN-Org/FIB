#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;

uniform mat4 modelViewProjectionMatrix, modelViewMatrix;
uniform mat3 normalMatrix;

uniform bool eyespace;
uniform vec3 boundingBoxMin, boundingBoxMax;
uniform float time;


void main()
{
	float Y = vertex.y;

	if(eyespace) 
		Y= (modelViewMatrix * vec4(vertex, 1.0)).y;

	float d = ((length(boundingBoxMax - boundingBoxMin)/2.0) / 10.0)*Y;

	frontColor = vec4(color,1.0);
	
	vec3 pos = vertex  + d * sin(time)*normal;

	gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
}
