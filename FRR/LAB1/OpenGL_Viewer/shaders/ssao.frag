#version 330 core

in vec2 TexCoords;

out vec4 frag_color;

uniform sampler2D gNormal;
uniform sampler2D gPosition;
uniform sampler2D texNoise;

uniform int sampleN;

uniform vec3 samples[64];
uniform mat4 projection;

uniform float radius;

uniform float width;
uniform float height;

//tile noise texture over screen based on screen dimensions divided by noise size
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
        // get sample position
        vec3 sample = TBN * samples[i]; // From tangent to view-space

        sample = fragPos + sample * radius;

        vec4 offset = vec4(sample, 1.0);
        offset      = projection * offset;    // from view to clip-space
        offset.xyz /= offset.w;               // perspective divide
        offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0


        float sampleDepth = texture(gPosition, offset.xy).z;

        if (fragPos.z < sampleDepth && abs(fragPos.z - sampleDepth) < radius) {
            occlusion+=1;
        }


    }

    occlusion = 1.0 - (occlusion / sampleN);
    frag_color = vec4(occlusion,occlusion,occlusion,1.0);



}
