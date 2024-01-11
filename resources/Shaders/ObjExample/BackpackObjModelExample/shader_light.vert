#version 450

layout (location = 0) out vec3 frag_color;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tex_coord;
layout (location = 4) in vec4 inTangent;

layout (set = 0, binding = 0) uniform ModelBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} mbo;

void main() {
    gl_Position = mbo.projection * mbo.view * mbo.model * vec4(position, 1.0);
    frag_color = color;
}