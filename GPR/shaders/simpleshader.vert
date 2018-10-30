#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 colorC;

out vec3 fragNormal;
out vec3 vColor;

uniform mat4 projection, modelview;
uniform mat3 normalMatrix;


void main()
{
        vColor = colorC;
	gl_Position = projection * modelview * vec4(position, 1.0);
	fragNormal = normalMatrix * normal;
}

