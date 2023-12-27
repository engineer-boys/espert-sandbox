#version 450
// out
layout (location = 0) out vec3 WorldPos;
layout (location = 1) out vec3 Normal;

// in
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tex_coord;

// DATA ----------------------------------
layout (push_constant) uniform Material {
    vec3 moved;
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
} material;

layout (binding = 0) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 camPos;
} mpv;

void main()
{
    Normal = normal;

    WorldPos = vec3(mpv.model * vec4(position, 1.0)) + material.moved;
    gl_Position = mpv.proj * mpv.view * vec4(WorldPos, 1.0);
}