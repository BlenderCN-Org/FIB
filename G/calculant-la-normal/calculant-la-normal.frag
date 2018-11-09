#version 330 core

in vec4 frontColor;
out vec4 fragColor;

in vec3 vPosition;

void main()
{
    vec3 v1 = dFdx(vPosition);
    vec3 v2 = dFdy(vPosition);
    vec3 n = normalize(cross(v1,v2));
    fragColor = frontColor*n.z;
}
