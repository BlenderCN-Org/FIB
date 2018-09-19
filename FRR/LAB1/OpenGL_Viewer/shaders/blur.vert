#version 330 core
layout (location = 0) in vec3 vert;
layout (location = 1) in vec2 texCoords;

uniform mat4 q_projection;
uniform mat4 q_view;


out vec2 TexCoords ;

void main(void)  {

    TexCoords = texCoords;

    vec4 view_vertex = q_view * vec4(vert, 1);

    gl_Position = q_projection * view_vertex;
}
