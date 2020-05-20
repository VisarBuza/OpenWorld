#version 430 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoords;

/* Uniforms */
layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;
layout(location = 3) uniform mat3 normal_matrix;
layout(location = 4) uniform float density;

/* Outputs */
layout(location = 0) out vec3 normal;
layout(location = 1) out vec2 vs_texcoords;
layout(location = 2) out vec3 fragPos;
layout(location = 3) out float visibility;

const float gradient = 1.5;

void main()
{
  fragPos = vec3(model * vec4(a_pos, 1.0));
  vec4 positionRelativeToCam = view * vec4(fragPos, 1.0);
	gl_Position = projection * positionRelativeToCam;
  normal = normal_matrix * a_normal;
  vs_texcoords = a_texcoords;

  float distance = length(positionRelativeToCam.xyz);
  visibility = exp(-pow((distance * density), gradient));
  visibility = clamp(visibility, 0.0, 1.0);
}