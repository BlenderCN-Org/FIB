#version 330 core

in vec2 vtexCoord;
out vec4 fragColor;

uniform sampler2D noise0;
uniform sampler2D rock1;
uniform sampler2D grass2;

void main()
{
    float noise = texture(noise0,vtexCoord).x;
    vec4 c0 = texture(rock1,vtexCoord);
    vec4 c1 = texture(grass2,vtexCoord);
    fragColor = mix(c0,c1,noise);
}
