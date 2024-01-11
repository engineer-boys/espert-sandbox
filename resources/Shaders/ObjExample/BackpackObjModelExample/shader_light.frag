#version 450

layout (location = 0) out vec4 out_color;

layout (location = 0) in vec3 frag_color;

void main() {
    vec3 color = pow(frag_color, vec3(1.0 / 2.2));

    out_color = vec4(color, 1.0);
}