#version 330 core

in vec3 fragNormal;

out vec4 fragColor;
uniform vec3 color;

in vec3 fcolor;

void main()
{

    fragColor = vec4(fcolor*fragNormal.z,1.0);
}
