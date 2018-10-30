#version 330 core


in vec3 fragNormal;
in vec3 vColor;

out vec4 fragColor;


uniform bool bLighting;
uniform vec4 color;


void main()
{
//        if(bLighting)
//                fragColor = color * normalize(fragNormal).z;
//        else
//                fragColor = color;

    fragColor=vec4(vColor,1.0);
}
