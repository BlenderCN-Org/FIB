#version 330

smooth in vec3 world_vertex;


uniform samplerCube specular_map;


vec3 color = vec3(0.6);

out vec4 frag_color;

void main (void) {
 vec3 V = normalize(world_vertex);
 vec3 gamma = pow(texture(specular_map,V).rgb,vec3(1.0/2.2));
 frag_color = vec4(gamma,texture(specular_map,V).a);
}

