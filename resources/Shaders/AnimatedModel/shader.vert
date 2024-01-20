#version 450

#define MAX_BONES  100

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tex_coord;
layout (location = 4) in ivec4 bone_id;
layout (location = 5) in vec4 weights;


layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec2 outUV;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;

layout (set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout (set = 0, binding = 1) uniform BonesMatrices {
    mat4 matrices[MAX_BONES];
} bones_matrices;



vec4 lightPos = vec4(5.0, 5.0, 5.0, 1.0);

void main() {
    outColor = color;
    outUV = tex_coord;

    mat4 BoneTransform = bones_matrices.matrices[bone_id[0]] * weights[0];
    BoneTransform += bones_matrices.matrices[bone_id[1]] * weights[1];
    BoneTransform += bones_matrices.matrices[bone_id[2]] * weights[2];
    BoneTransform += bones_matrices.matrices[bone_id[3]] * weights[3];
    gl_Position =  ubo.proj * ubo.view * BoneTransform * vec4(position, 1.0f);

    outNormal = normalize(transpose(inverse(mat3(ubo.view * BoneTransform))) * normal);

    vec4 pos = ubo.view * vec4(position, 1.0);
	vec3 lPos = mat3(ubo.view) * lightPos.xyz;
	outLightVec = lPos - pos.xyz;
	outViewVec = -pos.xyz;
}