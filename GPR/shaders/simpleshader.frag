#version 330 core


in vec3 fragNormal;
in vec4 vColor;

out vec4 fragColor;


uniform bool bLighting;
uniform vec4 color;


void main()
{
        if(bLighting)
                fragColor = vColor * normalize(fragNormal).z;
        else
                fragColor = color;

}
