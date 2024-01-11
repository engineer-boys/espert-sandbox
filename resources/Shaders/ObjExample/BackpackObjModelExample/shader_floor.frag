#version 450

layout (location = 1) in vec2 frag_tex_coord;
layout (location = 2) in vec3 frag_position;
layout (location = 3) in vec3 frag_normal;

layout (location = 0) out vec4 out_color;

layout (set = 1, binding = 0) uniform sampler2D albedo_texture;

layout (set = 0, binding = 1) uniform LightBufferObject {
    vec3 position;

    vec3 diffuse;
    vec3 specular;

    vec3 attenuation_strength;
} lbo;

layout (set = 0, binding = 2) uniform CameraBufferObject {
    vec3 position;
} cbo;

float ambient_strength = 0.5;

void main() {
    vec3 albedo = texture(albedo_texture, frag_tex_coord * 10).rgb;

    // lightning
    vec3 frag_normal_normalized = normalize(frag_normal);
    vec3 light_dir = normalize(lbo.position - frag_position);
    vec3 view_dir = normalize(cbo.position - frag_position);
    vec3 reflect_dir = reflect(-light_dir, frag_normal_normalized);

    vec3 ambient = ambient_strength * lbo.diffuse;
    vec3 diffuse = max(dot(frag_normal_normalized, light_dir), 0.0) * lbo.diffuse;
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0), 32) * lbo.diffuse;

    // attenuation
    float light_distance = length(lbo.position - frag_position);
    float attenuation = 1.0 / (lbo.attenuation_strength.x +
    lbo.attenuation_strength.y * light_distance +
    lbo.attenuation_strength.z * light_distance * light_distance);

    vec3 frag_color = (ambient + diffuse + specular) * attenuation * albedo;

    // gamma correction
    vec3 g_color = pow(frag_color, vec3(1.0 / 2.2));

    out_color = vec4(g_color, 1.0);
}