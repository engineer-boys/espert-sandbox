#version 450
// out
layout (location = 0) out vec3 WorldPos;
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec2 TexCoords;

// in
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tex_coord;
layout (location = 4) in vec4 inTangent;

// DATA ----------------------------------
layout (push_constant) uniform MovedVar {
    vec3 moved;
} mvd;

layout (binding = 0) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 camPos;
} mpv;

void main()
{
    Normal = normal;
    TexCoords = tex_coord;

    WorldPos = vec3(mpv.model * vec4(position, 1.0)) + mvd.moved;
    gl_Position = mpv.proj * mpv.view * vec4(WorldPos, 1.0);
}