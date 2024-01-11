#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tex_coord;
layout (location = 4) in vec4 inTangent;

layout (location = 1) out vec2 frag_tex_coord;
layout (location = 2) out vec3 frag_position;
layout (location = 3) out vec3 frag_normal;

layout (set = 0, binding = 0) uniform ModelBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} mbo;

void main() {
    vec4 model_position = mbo.model * vec4(position, 1.0);

    gl_Position = mbo.projection * mbo.view * model_position;
    frag_tex_coord = tex_coord;
    frag_position = model_position.xyz;
    frag_normal = mat3(transpose(inverse(mbo.model))) * normal;
}