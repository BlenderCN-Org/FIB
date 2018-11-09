#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform vec3 boundingBoxMax;
uniform vec3 boundingBoxMin;


vec3 defColor(float y){  //Compute the color depending on y value

  vec3 red = vec3(1.0,0.0,0.0);
  vec3 yellow = vec3(1.0,1.0,0.0);
  vec3 green = vec3(0.0,1.0,0.0);
  vec3 cyan = vec3(0.0,1.0,1.0);
  vec3 blue = vec3(0.0,0.0,1.0);

  vec3 mixedColor;

  float minY = boundingBoxMin.y;
  float maxY = boundingBoxMax.y;

  float val = 4*((y - minY) / (maxY-minY));

  if (val<1)
    mixedColor = mix(red,yellow,fract(val));
  if (val>=1 && val<2)
    mixedColor = mix(yellow,green,fract(val));
  if (val>=2 && val<3)
    mixedColor = mix(green,cyan,fract(val));
  if (val>=3 && val<4)
    mixedColor = mix(cyan,blue,fract(val)); 
  if (val>=4)
    mixedColor = blue;

return mixedColor;

}

void main()
{
    frontColor = vec4(defColor(vertex.y),1.0);
    gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
}
