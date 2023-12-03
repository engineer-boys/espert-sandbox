#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tex_coord;

layout (location = 0) out vec3 fragColor;

layout (push_constant) uniform Push {
    mat4 view;
    mat4 proj;
} push;

layout (set = 0, binding = 0) uniform Ubo {
    mat4 model;
} ubo;

void main()
{
    gl_Position = push.proj * push.view * ubo.model * vec4(position, 1.0);
    fragColor = color;
}
