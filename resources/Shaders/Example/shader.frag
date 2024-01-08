#version 450

layout (location = 0) in vec3 fragColor;

layout (location = 0) out vec4 outColor;

layout (push_constant) uniform Push {
    layout (offset = 16) vec3 color;
} push;

void main()
{
    vec3 g_color = pow(fragColor * push.color, vec3(1.0/2.2)); 
    outColor = vec4(g_color, 1.0);
}