#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform float time;
const float PI = 3.14159;

void main()
{
    	vec3 N = normalize(normalMatrix * normal);
    	frontColor = vec4(vec3(N.z),1.0);

	vec3 pos = vertex;
	pos.z += 0.25*sin(2*PI*time + 0.5*pos.x)*(1-smoothstep(0.5,0.75,texCoord.s));

    	vtexCoord = texCoord;
    	gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
}
