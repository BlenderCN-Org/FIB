#version 330 core


in vec3 fragNormal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec4 color;
uniform float radius;

in vec2 val;


void main()
{

    float R = radius;
    float dist = sqrt(dot(val,val));
    if (dist >= R) {
        discard;
    }

    fragColor = color;


}
