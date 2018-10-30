#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 2) in vec2 value;
out vec2 val;

out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 projection, modelview;
uniform mat3 normalMatrix;

uniform vec3 offset;


void main()
{
        val = value;
        gl_Position = projection * modelview * vec4(offset,1.0);
        gl_Position.xyz += position;
        fragNormal = normalMatrix * normal;
        //fragPos = (modelview * vec4(position,1.0)).xyz;
//        gl_PointSize =  4.0;

}

