#version 450

layout (location = 0) in vec3 fragColor;

layout (location = 0) out vec4 outColor;

layout (push_constant) uniform Push {
    layout (offset = 16) vec3 color;
} push;

layout (constant_id = 0) const bool USE_SPECIALIZED_COLOR = false;
layout (constant_id = 1) const float SPECIALIZED_COLOR_R = 0.0;
layout (constant_id = 2) const float SPECIALIZED_COLOR_G = 0.0;
layout (constant_id = 3) const float SPECIALIZED_COLOR_B = 0.0;

void main()
{
    if (USE_SPECIALIZED_COLOR) {
        outColor = vec4(SPECIALIZED_COLOR_R, SPECIALIZED_COLOR_G, SPECIALIZED_COLOR_B, 1.0);
    }
    else {
        vec3 g_color = pow(fragColor * push.color, vec3(1.0/2.2)); 
        outColor = vec4(g_color, 1.0);
    }
}