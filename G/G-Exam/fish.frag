#version 330 core

in vec4 frontColor;
out vec4 fragColor;
in vec2 vtexCoord;

uniform sampler2D fishColor;

void main()
{
    fragColor = texture(fishColor,vtexCoord)*frontColor;
}
