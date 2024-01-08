#version 450

layout (location = 0) in vec3 inPos;

layout (location = 0) out vec3 outUVW;

layout (binding = 0) uniform UBO 
{
    mat4 view;
	mat4 projection;
} ubo;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	outUVW = inPos;
    vec4 pos = ubo.projection * ubo.view * vec4(inPos.xyz, 1.0);
	gl_Position = pos.xyww;
}
