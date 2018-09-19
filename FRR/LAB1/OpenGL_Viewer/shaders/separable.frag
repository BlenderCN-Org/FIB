#version 330 core

in vec2 TexCoords;

out vec4 frag_color;

uniform sampler2D gNormal;
uniform sampler2D gPosition;
uniform sampler2D texNoise;

uniform int sampleN;

uniform vec3 samples_x[64];
uniform vec3 samples_y[64];
uniform mat4 projection;

uniform float radius;

uniform float width;
uniform float height;

vec2 noiseScale = vec2(width/4.0, height/4.0);

void main(void)
{
    vec3 fragPos = texture(gPosition,TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb *2 -1);
    vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;

    vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN       = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;

    for(int i = 0; i < sampleN; ++i)
    {

        vec3 sample = TBN * samples_x[i];

        sample = fragPos + sample * radius;

        vec4 offset = vec4(sample, 1.0);
        offset      = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz  = offset.xyz * 0.5 + 0.5;


        float sampleDepth = texture(gPosition, offset.xy).z;

        if (fragPos.z < sampleDepth && abs(fragPos.z - sampleDepth) < radius) {
            occlusion+=1;
        }


    }

    for(int i = 0; i < sampleN; ++i)
    {

        vec3 sample = TBN * samples_y[i];

        sample = fragPos + sample * radius;

        vec4 offset = vec4(sample, 1.0);
        offset      = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz  = offset.xyz * 0.5 + 0.5;


        float sampleDepth = texture(gPosition, offset.xy).z;

        if (fragPos.z < sampleDepth && abs(fragPos.z - sampleDepth) < radius) {
            occlusion+=1;
        }


    }


    occlusion = 1.0 - (occlusion / (2*float(sampleN)));
    frag_color = vec4(occlusion,occlusion,occlusion,1.0);



}

