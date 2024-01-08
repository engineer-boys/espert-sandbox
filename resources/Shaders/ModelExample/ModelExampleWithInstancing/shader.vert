#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tex_coord;
layout (location = 4) in vec4 inTangent;
layout (location = 5) in mat4 model;

layout (location = 0) out vec3 fragColor;

layout (push_constant) uniform Push {
    mat4 view;
    mat4 proj;
} push;

void main()
{
    gl_Position = push.proj * push.view * model * vec4(position, 1.0);
    fragColor = color;
}
