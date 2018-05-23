#version 330 core
layout (location = 0) in vec3 vert;

out vec2 TexCoords;


void main(void)  {

    vec2 positionIn = vec2(vert.x,vert.y);
    TexCoords = (positionIn * vec2(0.5)) + vec2(0.5);
    gl_Position = vec4(vert,1.0);


}



