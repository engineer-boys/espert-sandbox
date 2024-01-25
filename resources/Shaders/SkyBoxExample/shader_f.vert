#version 450
// in
layout (location = 0) in vec3 position;

// DATA ----------------------------------
layout (binding = 0) uniform MVPC {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 camPos;
} mpvc;

void main()
{
    gl_Position = mpvc.proj * mpvc.view * mpvc.model * vec4(position, 1.0);
}
