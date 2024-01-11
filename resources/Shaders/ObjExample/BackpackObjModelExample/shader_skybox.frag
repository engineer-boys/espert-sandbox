#version 450 core
layout (location = 0) out vec4 out_color;

layout (location = 0) in vec3 frag_tex_coord;

layout (binding = 1) uniform samplerCube skybox;

void main()
{
    out_color = vec4(pow(texture(skybox, frag_tex_coord).xyz, vec3(0.454545454545)), 1);
}