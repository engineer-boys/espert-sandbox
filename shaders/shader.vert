#version 450

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 instance_position;
layout (location = 2) in vec3 color;

layout (location = 0) out vec3 fragColor;
void main()
{
    gl_Position = vec4(instance_position + position, 0.5, 1.0);
    fragColor = color;
}