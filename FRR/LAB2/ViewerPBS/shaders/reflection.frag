#version 330

smooth in vec3 world_normal;
smooth in vec3 world_vertex;
flat in vec3 world_camera_pos;

uniform samplerCube specular_map;

uniform mat4 view;
out vec4 frag_color;


void main()
{
    vec3 N = normalize(world_normal);
    vec3 I = normalize(world_camera_pos - world_vertex);
    vec3 R = reflect(-I,N);

    vec3 gamma = pow(texture(specular_map,R).rgb,vec3(1.0/2.2));
    frag_color = vec4(gamma,1.0);

}
