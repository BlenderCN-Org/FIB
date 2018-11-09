#version 330 core

uniform mat4 modelViewMatrixInverse;

out vec4 fragColor;

in vec3 N;   	// normal (en object space)
in vec3 P; 	// posicio del vertex (en object space)

uniform float time;
uniform bool rotate;

// V, N, P, lightPos han d'estar al mateix espai de coordenades
// V és el vector unitari cap a l'observador
// N és la normal
// P és la posició 
// lightPos és la posició de la llum
// lightColor és el color de la llum
vec4 light(vec3 V, vec3 N, vec3 P, vec3 lightPos, vec3 lightColor)
{
	const float shininess = 100.0;
	const float Kd = 0.5;
	N = normalize(N);
	vec3 L = normalize(lightPos - P);
	vec3 R = reflect(-L, N);
	float NdotL = max(0.0, dot(N,L));
	float RdotV = max(0.0, dot(R,V));
	float spec =  pow( RdotV, shininess);
	return vec4(Kd*lightColor*NdotL + vec3(spec),0);
}

void main()
{
	float angle = 0;
	if (rotate) angle = time;
	float cs = cos(angle);
	float sn = sin(angle);
	mat3 R = mat3(vec3(cs, sn, 0), vec3(-sn, cs,0), vec3(0,0,1));
	
	vec3 eyeL1 = vec3(0.0,10.0,0.0);
	vec3 eyeL2 = vec3(0.0,-10.0,0.0);
	vec3 eyeL3 = vec3(10.0,0.0,0.0);
	vec3 eyeL4 = vec3(-10.0,0.0,0.0);

	if(rotate){
		eyeL1 = R * eyeL1;
		eyeL2 = R * eyeL2;
		eyeL3 = R * eyeL3;
		eyeL4 = R * eyeL4;
	}

	vec3 L1 = (modelViewMatrixInverse*vec4(eyeL1,1.0)).xyz;
	vec3 L2 = (modelViewMatrixInverse*vec4(eyeL2,1.0)).xyz;
	vec3 L3 = (modelViewMatrixInverse*vec4(eyeL3,1.0)).xyz;
	vec3 L4 = (modelViewMatrixInverse*vec4(eyeL4,1.0)).xyz;

	vec3 verd = vec3(0.0,1.0,0.0);
	vec3 groc = vec3(1.0,1.0,0.0);
	vec3 blau = vec3(0.0,0.0,1.0);
	vec3 vermell = vec3(1.0,0.0,0.0);

	vec3 V = normalize(modelViewMatrixInverse[3].xyz - P);
	vec4 c = vec4(0);
	c += light(V, N, P, L1, verd);
	c += light(V, N, P, L2, groc);
	c += light(V, N, P, L3, blau);
	c += light(V, N, P, L4, vermell);
	
	fragColor = c;
}

