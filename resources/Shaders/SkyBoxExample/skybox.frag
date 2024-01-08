#version 450 core
layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec3 TexCoords;

layout(binding = 1) uniform samplerCube skybox;

void main()
{    
    FragColor = vec4(pow(texture(skybox, TexCoords).xyz, vec3(0.454545454545)), 1);
}