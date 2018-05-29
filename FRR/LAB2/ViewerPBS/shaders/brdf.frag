#version 330

smooth in vec3 world_normal;
smooth in vec3 world_vertex;
flat in vec3 world_camera_pos;

uniform samplerCube specular_map;
uniform samplerCube diffuse_map;
uniform vec3 fresnel;
uniform mat4 view;

out vec4 frag_color;

float alpha = 0.3;


//-----------------------------------------------------------------
//--------------- Fresnel Schlick approximation -------------------
//-----------------------------------------------------------------


vec3 FresnelSchlick(vec3 F0, vec3  l, vec3 h)
{
   return F0 + (vec3(1.0,1.0,1.0)-F0) * pow((1-dot(l,h)),5.0);

}

//-----------------------------------------------------------------
//-------------------- Geometry functions -------------------------
//-----------------------------------------------------------------

float Geometry(vec3 n, vec3 v)
{
    float k = alpha * alpha / 2;
    return (dot(n,v) / (dot(n,v)*(1-k)+k));
}

float G2(vec3 n, vec3 v, vec3 l){
    float NdotV = max(dot(n,v),0.0);
    float NdotL = max(dot(n,l),0.0);
    float gg1 = Geometry(n,v);
    float gg2 = Geometry(n,l);
    return gg1*gg2;
}

float G3(vec3 n, vec3 l, vec3 v, vec3 h){
    return min(1,min((2*dot(n,h)*dot(n,v)/dot(v,h)),(2*dot(n,h)*dot(n,l)/dot(v,h))));
}

//-----------------------------------------------------------------

void main()
{
    //Vector definitions in world space
    vec3 n = normalize(world_normal);
    vec3 v = normalize(world_camera_pos - world_vertex);
    vec3 l = reflect(-v,n);
    vec3 h = normalize(l+v);

    //Intensity of F0
    float I = (fresnel.r + fresnel.g + fresnel.b)/3.0;

    //Fresnel reflectance
    vec3 F = FresnelSchlick(fresnel,n,l) + 0.4*(1-I);

    //Geometry functions
    float G = 1;
    float G_2 = G2(n,v,l);
    float G_3 = G3(n,l,v,h);

    //Normal distribution with gamma correction
    vec3 D = pow(texture(diffuse_map,l).rgb,vec3(1.0/2.2));

    //BRDF Formula
    vec3 color = (1-I)*D + F*G*D / (4*dot(n,l)*dot(n,l));

    frag_color = vec4(color,1.0);

}
