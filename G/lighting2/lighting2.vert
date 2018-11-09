#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matShininess;
uniform vec4 lightAmbient, lightDiffuse, lightSpecular, lightPosition;


vec4 light(vec3 N, vec3 V, vec3 L)
{
	N = normalize(N);
	V = normalize(V);
	L = normalize(L);

	vec3 R = normalize(2.0*dot(N,L)*N-L);
	float NdotL = max(0.0,dot(N,L));
	float RdotV = max(0.0,dot(R,V));

	vec4 ambient = matAmbient * lightAmbient;
	vec4 diffuse = matDiffuse * lightDiffuse * NdotL;

	float Ispec = 0;
	if (NdotL>0) Ispec = pow(RdotV,matShininess);
	vec4 specular = matSpecular * lightSpecular * Ispec;

	return ambient + diffuse + specular;

}

void main()
{
	vec3 N = normalize(normalMatrix * normal);
	vec3 P = (modelViewMatrix * vec4(vertex,1.0)).xyz;
	vec3 L = (lightPosition.xyz - P);
	vec3 V = -P;

	frontColor = light(N,V,L);
	vtexCoord = texCoord;
	gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
}
