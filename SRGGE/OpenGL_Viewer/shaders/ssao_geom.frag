#version 330 core
layout (location = 0) out vec3 gNormal;
layout (location = 1) out vec3 gPosition;

in vec3 N;
in vec3 V;

out vec4 frag_color;

void main(void)
{
     gPosition=V;
     gNormal = (normalize(N)+1.0)/2;

}
