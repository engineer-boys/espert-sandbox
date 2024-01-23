#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inUV;

layout (set = 0, binding = 0) uniform UBOScene
{
	mat4 model;
	mat4 projection;
	mat4 view;
	vec4 lightPos;
	vec4 viewPos;
} uboScene;

layout(push_constant) uniform PushConsts {
	mat4 transform;
} primitive;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec2 outUV;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;

void main()
{
	mat4 world_model = uboScene.model * primitive.transform;
	vec4 world_pos = uboScene.view * world_model * vec4(inPos, 1.0);

	outNormal = inNormal;
	outColor = inColor;
	outUV = inUV;
	gl_Position = uboScene.projection * world_pos;

	vec3 lPos = mat3(uboScene.view) * uboScene.lightPos.xyz;
	outLightVec = uboScene.lightPos.xyz - world_pos.xyz;
	outViewVec = uboScene.viewPos.xyz - world_pos.xyz;
}