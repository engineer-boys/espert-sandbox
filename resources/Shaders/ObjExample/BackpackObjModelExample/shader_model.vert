#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tex_coord;
layout (location = 4) in vec4 inTangent;

layout (location = 0) out vec2 frag_tex_coord;
layout (location = 1) out vec3 frag_position;
layout (location = 2) out vec3 frag_normal;
layout (location = 3) out vec4 frag_position_light_space;

layout (set = 0, binding = 0) uniform ModelBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} mbo;

layout (set = 0, binding = 3) uniform LightSpaceBufferObject {
    mat4 mat;
} lsbo;

const mat4 biasMat = mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 1.0);

void main() {
    vec4 model_position = mbo.model * vec4(position, 1.0);

    gl_Position = mbo.projection * mbo.view * model_position;

    frag_tex_coord = tex_coord;
    frag_position = model_position.xyz;
    frag_normal = mat3(transpose(inverse(mbo.model))) * normal;
    frag_position_light_space = biasMat * lsbo.mat * vec4(frag_position, 1.0);
}