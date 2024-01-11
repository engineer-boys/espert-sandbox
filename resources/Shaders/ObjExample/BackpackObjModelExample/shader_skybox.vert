#version 450 core

layout (location = 0) in vec3 position;

layout (location = 0) out vec3 frag_tex_coord;

layout (set = 0, binding = 0) uniform SkyboxUniform {
    mat4 view;
    mat4 projection;
} su;

void main()
{
    frag_tex_coord = position;
    vec4 pos = su.projection * su.view * vec4(position, 1.0);
    gl_Position = pos.xyww;
}