#version 430 core
/** Outputs */
layout(location = 0) out vec4 out_color;

/** Inputs */    
layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 vs_texcoords;
layout(location = 2) in vec3 fragPos;

/** Texture sampler */
layout(binding = 0) uniform sampler2D texture_diffuse;

void main()
{
  out_color = vec4(vec3(0.01f, 0.2f, 0.4f), 1.0);
}

