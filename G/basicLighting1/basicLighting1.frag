#version 330 core

in vec4 frontColor;
out vec4 fragColor; 

uniform float time;

void main()
{
    fragColor = frontColor;
}
