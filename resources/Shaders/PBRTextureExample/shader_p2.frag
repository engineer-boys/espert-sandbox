#version 450

layout(binding = 0) uniform sampler2D texSampler;

layout (location = 0) in vec2 fragTexCoord;
layout (location = 0) out vec4 outColor;

void main()
{
    // gamma
    vec3 color = pow(texture(texSampler, fragTexCoord).xyz, vec3(1.0/2.2)); 
    
    outColor = vec4(color, 1.0);
}