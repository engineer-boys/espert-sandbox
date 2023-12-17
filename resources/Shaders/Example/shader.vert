#version 450

layout (set = 0, binding = 0) uniform MV {
    mat4 model;
    mat4 view;
} mv;

layout (set = 1, binding = 0) uniform P_matrix {
    mat4 proj;
} p_matrix;

layout (push_constant) uniform Push {
    vec2 move;
} push;

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

layout (location = 0) out vec3 fragColor;
void main()
{
    gl_Position = p_matrix.proj * mv.view * mv.model * vec4(position, 0.0, 1.0) + vec4(push.move, 0.0, 1.0);
    fragColor = color;
}