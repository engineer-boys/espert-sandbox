#version 450

layout (location = 1) in vec2 frag_tex_coord;

layout (location = 0) out vec4 out_color;

layout (set = 1, binding = 0) uniform sampler2D albedo_texture;

void main() {
    vec3 albedo = texture(albedo_texture, frag_tex_coord).rgb;

    //    out_color = vec4(ao + albedo + normal + roughness + metallic, 1.0);
    vec3 color = pow(albedo, vec3(1.0/2.2)); 
    out_color = vec4(color, 1.0);
}