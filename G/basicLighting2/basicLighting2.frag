#version 330 core

in vec4 frontColor;
out vec4 fragColor;
in vec3 vN;

uniform float time;

void main()
{
    vec3 N=normalize(vN);
    fragColor = frontColor*N.z;
} 
