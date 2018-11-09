#version 330 core

out vec4 fragColor;
in vec2 vtexCoord;

void main()
{
  if(fract(vtexCoord.s)>0.1 && fract(vtexCoord.t)>0.1)
    fragColor = vec4(0.8);
  else
    fragColor = vec4(vec3(0),1.0);
}
