#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec2 vtexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform float time;

void main()
{
    float slice = 1.0/30;
    int frame = int(mod(time/slice,48));
    vtexCoord = vec2((texCoord.s+mod(frame,8))/8,(texCoord.t+(5-frame/8))/6);
    gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
}
