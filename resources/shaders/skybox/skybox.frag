#version 430 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform samplerCube skybox2;
uniform float blendFactor;
uniform vec3 fogColor;

const float lowerLimit = 0.0;
const float upperLimit = 0.8;

void main()
{    
    vec4 texture1 = texture(skybox, TexCoords);
    vec4 texture2 = texture(skybox2, TexCoords);
    vec4 finalColor = mix(texture1, texture2, blendFactor);

    float factor = (TexCoords.y - lowerLimit) / (upperLimit - lowerLimit);
    factor = clamp(factor, 0.0, 1.0);
    
    FragColor = mix(vec4(fogColor, 1.0), finalColor, factor);
}