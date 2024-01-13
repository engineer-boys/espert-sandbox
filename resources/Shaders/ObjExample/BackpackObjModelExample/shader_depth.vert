#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tex_coord;
layout (location = 4) in vec4 inTangent;


layout (set = 0, binding = 0) uniform BufferObject {
    mat4 model;
    mat4 light_space_mat;
} bo;

void main()
{
    vec3 flipped_position = vec3(position.x, -position.y, position.z); // TODO: don't flip position inside shader
    gl_Position = bo.light_space_mat * bo.model * vec4(flipped_position, 1.0);
}