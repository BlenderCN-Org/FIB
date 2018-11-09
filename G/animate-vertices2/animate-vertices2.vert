#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

const float PI = 3.1415926535897932384626433832795;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform float time;
uniform float amplitude=0.1;
uniform float freq=1; //expressada en Hz

void main()
{
    vec3 N = normalize(normalMatrix * normal);
    frontColor = vec4(vec3(N.z),1.0);
    vtexCoord = texCoord;

    vec3 position = vertex + amplitude*sin(2*PI*freq*time+2*PI*texCoord.s)*normal;
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}
