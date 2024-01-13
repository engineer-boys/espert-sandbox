#version 450

layout (location = 0) in vec2 frag_tex_coord;
layout (location = 1) in vec3 frag_position;
layout (location = 2) in vec3 frag_normal;
layout (location = 3) in vec4 frag_position_light_space;

layout (location = 0) out vec4 out_color;

layout (set = 0, binding = 0) uniform ModelBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    float sample_offset;
    bool calculate_lightning;
    bool calculate_shadows;
} mbo;

layout (set = 0, binding = 1) uniform LightBufferObject {
    vec3 position;

    vec3 diffuse;
    vec3 specular;

    vec3 attenuation_strength;
} lbo;

layout (set = 0, binding = 2) uniform CameraBufferObject {
    vec3 position;
} cbo;

layout (set = 1, binding = 0) uniform sampler2D albedo_texture;
layout (set = 1, binding = 1) uniform sampler2D normal_texture;
layout (set = 1, binding = 2) uniform sampler2D metallic_texture;
layout (set = 1, binding = 3) uniform sampler2D roughness_texture;
layout (set = 1, binding = 4) uniform sampler2D ao_texture;

layout (set = 2, binding = 0) uniform sampler2D depth_map;

float ambient_strength = 0.5;

float sample_depth_map(vec4 frag_pos_light);
float calculate_shadow_with_filter_PCF(vec4 sc);

void main() {
    vec3 frag_color = texture(albedo_texture, frag_tex_coord * mbo.sample_offset).rgb;

    if (mbo.calculate_lightning)
    {
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

        frag_color *= (ambient + diffuse + specular) * attenuation;
    }

    if (mbo.calculate_shadows)
    {
        // shadows
        float shadow = calculate_shadow_with_filter_PCF(frag_position_light_space);

        frag_color *= shadow;
    }

    // gamma correction
    vec3 g_color = pow(frag_color, vec3(1.0 / 2.2));

    out_color = vec4(g_color, 1.0);
}

float shadow_offset = 0.025;
float shadow_strength = 0.25;
float sample_depth_map(vec4 frag_pos_light, vec2 offset)
{
    float shadow = 1.0;

    frag_pos_light -= shadow_offset;
    frag_pos_light /= frag_pos_light.w;
    if (frag_pos_light.z > -1.0 && frag_pos_light.z < 1.0)
    {
        float dist = texture(depth_map, frag_pos_light.xy + offset).r;
        if (frag_pos_light.w > 0.0 && dist < frag_pos_light.z)
        {
            shadow = shadow_strength;
        }
    }

    return shadow;
}

float calculate_shadow_with_filter_PCF(vec4 sc)
{
    ivec2 tex_dim = textureSize(depth_map, 0);
    float scale = 1.5;
    float dx = scale * 1.0 / float(tex_dim.x);
    float dy = scale * 1.0 / float(tex_dim.y);

    float shadow_factor = 0.0;
    int count = 0;
    int range = 1;

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            shadow_factor += sample_depth_map(sc, vec2(dx * x, dy * y));
            count++;
        }

    }
    return shadow_factor / count;
}