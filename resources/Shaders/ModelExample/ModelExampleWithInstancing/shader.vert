#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tex_coord;
layout (location = 4) in mat4 model;

layout (location = 0) out vec3 fragColor;

layout (set = 0, binding = 0) uniform Ubo {
    mat4 view;
    mat4 proj;
} ubo;

//layout(push_constant) uniform Push {
//    mat4 transform;
//} push;

void main()
{
    //gl_Position = ubo.camera_projection * push.transform * vec4(position, 1.0);
    gl_Position = ubo.proj * ubo.view * model * vec4(position, 1.0);
    fragColor = color;
}
