#version 330 core

out vec4 frag_color;

in vec2 TexCoords;

uniform sampler2D ssaoInput;

uniform float blurRadius;
uniform float sigma;
uniform float width;
uniform float height;

uniform bool blur_flag;

void main()
{

    vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
    float nColor = 0.0;
    float weight_total = 0.0;

    if (blur_flag){



        for (int x = -2; x <= 2; ++x)
        {
            for (int y = -2; y <= 2; ++y)
            {
                float weight=exp(-((x*x + y*y)/sigma));
                vec2 offset = vec2(float(x) * blurRadius, float(y) * blurRadius) * texelSize;
                nColor += texture(ssaoInput, TexCoords + offset).r * weight;
                weight_total+=weight;

            }
        }

        float val = nColor / weight_total;

        frag_color = vec4(val, val, val, 1.0);
    }

    else{

        vec4 color = texture(ssaoInput,TexCoords);
        frag_color = color;

        }
}
