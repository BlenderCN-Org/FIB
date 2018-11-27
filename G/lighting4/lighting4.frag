#version 330 core

in vec4 frontColor;
out vec4 fragColor; 

in vec3 N;
in vec3 P;

uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matShininess;
uniform vec4 lightAmbient, lightDiffuse, lightSpecular, lightPosition;

uniform mat4 modelViewMatrix;

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
	vec3 L = (lightPosition).xyz - P;
	vec3 V = -P;
    	fragColor = light(N,V,L);

}