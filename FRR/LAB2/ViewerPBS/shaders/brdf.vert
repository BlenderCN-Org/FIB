#version 330

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal_matrix;

smooth out vec3 world_normal;
smooth out vec3 world_vertex;
flat out vec3 world_camera_pos;

void main(void)  {

    //Camera position in world space
    world_camera_pos = inverse(view)[3].xyz;

    //Vertex position in world space
    world_vertex = (model * vec4(vert,1)).xyz;

    //Normal vector in world space
    world_normal= normal;

    //Vertex position in clip space
    gl_Position = projection * view * model * vec4(vert, 1);
}
