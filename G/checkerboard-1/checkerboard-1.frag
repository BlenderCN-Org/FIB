#version 330 core

out vec4 fragColor;
in vec2 vtexCoord;

void main()
{
    if(int(mod(vtexCoord.s,2)) == int(mod(vtexCoord.t,2)))	
    	fragColor = vec4(0.8);
    else 	
    	fragColor = vec4(vec3(0),1.0);
}
