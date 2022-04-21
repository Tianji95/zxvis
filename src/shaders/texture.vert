#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inUV;

layout (binding = 0) uniform UBO 
{
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

layout (location = 0) out vec3 outColor;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{
	outColor = inColor;
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPos.xyz, 1.0);
}
