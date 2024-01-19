#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tex_coord;
layout (location = 4) in ivec4 bone_id;
layout (location = 5) in vec4 weights;


// layout (set = 0, binding = 0) uniform UBOScene
// {
// 	mat4 projection;
// 	mat4 view;
// 	vec4 lightPos;
// 	vec4 viewPos;
// } uboScene;

// layout(push_constant) uniform PushConsts {
// 	mat4 model;
// } primitive;

// layout( set = 1, binding = 0) uniform JointMatrices {
// 	mat4 jointMatrices[100];
// } matrices;

// layout (location = 0) out vec3 outNormal;
// layout (location = 1) out vec3 outColor;
// layout (location = 2) out vec2 outUV;
// layout (location = 3) out vec3 outViewVec;
// layout (location = 4) out vec3 outLightVec;

// void main() 
// {
// 	outNormal = inNormal;
// 	outColor = inColor;
// 	outUV = inUV;

// 	// Calculate skinned matrix from weights and joint indices of the current vertex
// 	mat4 skinMat = 
// 		inJointWeights.x * matrices.jointMatrices[int(inJointIndices.x)] +
// 		inJointWeights.y * matrices.jointMatrices[int(inJointIndices.y)] +
// 		inJointWeights.z * matrices.jointMatrices[int(inJointIndices.z)] +
// 		inJointWeights.w * matrices.jointMatrices[int(inJointIndices.w)];

// 	gl_Position = uboScene.projection * uboScene.view * primitive.model * skinMat * vec4(inPos.xyz, 1.0);
	
// 	outNormal = normalize(transpose(inverse(mat3(uboScene.view * primitive.model * skinMat))) * inNormal);

// 	vec4 pos = uboScene.view * vec4(inPos, 1.0);
// 	vec3 lPos = mat3(uboScene.view) * uboScene.lightPos.xyz;
// 	outLightVec = lPos - pos.xyz;
// 	outViewVec = -pos.xyz;
// }

// #version 450

#define MAX_BONES  100
#define MAX_BONE_INFLUENCE  4

// layout (location = 0) in vec3 position;
// layout (location = 1) in vec3 color;
// layout (location = 2) in vec3 normal;
// layout (location = 3) in vec2 tex_coord;
// layout (location = 4) in ivec4 bone_id;
// layout (location = 5) in vec4 weights;

layout (location = 1) out vec2 fragTexCoord;

layout (set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout (set = 0, binding = 1) uniform BonesMatrices {
    mat4 matrices[MAX_BONES];
} bones_matrices;

void main() {
    mat4 BoneTransform = bones_matrices.matrices[bone_id[0]] * weights[0];
    BoneTransform += bones_matrices.matrices[bone_id[1]] * weights[1];
    BoneTransform += bones_matrices.matrices[bone_id[2]] * weights[2];
    BoneTransform += bones_matrices.matrices[bone_id[3]] * weights[3];

    vec4 totalPosition = BoneTransform * vec4(position, 1.0f);

    mat4 viewModel = ubo.view * mat4(1);
    gl_Position =  ubo.proj * viewModel * totalPosition;
    fragTexCoord = tex_coord;
}