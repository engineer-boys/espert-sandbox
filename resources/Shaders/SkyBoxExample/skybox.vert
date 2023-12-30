#version 450 core
layout (location = 0) in vec3 aPos;

layout (location = 0) out vec3 TexCoords;

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

void main()
{
    TexCoords = aPos;
    vec4 pos = ubo.proj * ubo.view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}