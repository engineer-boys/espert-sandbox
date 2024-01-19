// #version 450

// layout (set = 2, binding = 0) uniform sampler2D samplerColorMap;

// layout (location = 0) in vec3 inNormal;
// layout (location = 1) in vec3 inColor;
// layout (location = 2) in vec2 inUV;
// layout (location = 3) in vec3 inViewVec;
// layout (location = 4) in vec3 inLightVec;

// layout (location = 0) out vec4 outFragColor;

// void main() 
// {
// 	vec4 color = texture(samplerColorMap, inUV) * vec4(inColor, 1.0);

// 	vec3 N = normalize(inNormal);
// 	vec3 L = normalize(inLightVec);
// 	vec3 V = normalize(inViewVec);
// 	vec3 R = reflect(-L, N);
// 	vec3 diffuse = max(dot(N, L), 0.5) * inColor;
// 	vec3 specular = pow(max(dot(R, V), 0.0), 16.0) * vec3(0.75);
// 	outFragColor = vec4(diffuse * color.rgb + specular, 1.0);		
// }

#version 450

layout (location = 1) in vec2 frag_tex_coord;

layout (location = 0) out vec4 out_color;

// layout (set = 1, binding = 0) uniform sampler2D albedo_texture;
// layout (set = 1, binding = 1) uniform sampler2D normal_texture;
// layout (set = 1, binding = 2) uniform sampler2D metallic_texture;
// layout (set = 1, binding = 3) uniform sampler2D roughness_texture;
// layout (set = 1, binding = 4) uniform sampler2D ao_texture;

void main() {
    // vec3 albedo, normal, metallic, roughness, ao;
    // albedo = texture(albedo_texture, frag_tex_coord).rgb;
    // normal = texture(normal_texture, frag_tex_coord).rgb;
    // metallic = texture(metallic_texture, frag_tex_coord).rgb;
    // roughness = texture(roughness_texture, frag_tex_coord).rgb;
    // ao = texture(ao_texture, frag_tex_coord).rgb;

    //    out_color = vec4(ao + albedo + normal + roughness + metallic, 1.0);
    vec3 color = pow(vec3(0.4, 0.4, 0.5), vec3(1.0/2.2)); 
    out_color = vec4(color, 1.0);
}