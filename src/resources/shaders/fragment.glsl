#version 430 core

in vec2 o_texCoord;
in vec3 o_position;
in vec4 o_color;
in vec4 o_fragPosition;
in vec3 o_normal;

out vec4 FragColor;

layout (binding = 0) uniform sampler2D   u_textureSampler2D;
layout (binding = 1) uniform samplerCube u_textureSampler3D;
uniform int         u_enableTexture = 0;

uniform int u_samplerSwitch = 0;

// Albedo color multiplier. Albedo means base color here.
uniform vec4 u_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

// Phong
uniform vec3 u_lightSourcePosition = vec3(1.0f);// Receives main:camera.getPosition()
uniform int u_enableLighting       = 1;

uniform vec3 u_cameraPosition = vec3(0.0f);// For specular illumination

/**
 * Material properties grouped into a single structure.
 */
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material u_material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light u_light;

void main() {
    vec4 tex = vec4(1.0f);
    vec4 baseColor;

    if (u_enableTexture == 1) {
        switch (u_samplerSwitch) {
            case 0:
            tex = texture(u_textureSampler2D, o_texCoord).rgba;
            break;
            case 1:
            tex = texture(u_textureSampler3D, o_position).rgba;
            break;
        }
        // Texture and base color
        baseColor = mix(tex, o_color * u_color, 0.4);
    } else {
        baseColor = o_color * u_color;
    }

    // Ambient lighting
    vec3 out_ambient = u_light.ambient * u_material.ambient;

    // Diffuse lighting
    vec3 norm = normalize(o_normal);
    vec3 lightDirection = normalize(u_lightSourcePosition - o_fragPosition.xyz);
    float diff =  max(dot(norm, lightDirection), 0.0f);
    vec3 out_diffuse = diff * u_light.diffuse * u_material.diffuse;

    // Specular lighting
    vec3 observerDirection = normalize(u_cameraPosition - o_fragPosition.xyz);
    vec3 reflectionDirection = reflect(-lightDirection, norm);

    /*
     * Specular formula: (Source: issue 22 by Rafael Palomar)
     *
     * Where str is specFactor, ref is reflectedLight, obs is
     * observerDirection, n is arbitrary shininess factor, and "."
     * signifies dot product. Here split across 2 lines of code.
     *
     * S = str . (ref . obs)^n
     */
    float specFactor = pow(max(dot(observerDirection, reflectionDirection), 0.0f), u_material.shininess);
    vec3  out_specular = specFactor * u_light.specular * u_material.specular;

    // Compute the final colors
    if (u_enableLighting == 1) {
        FragColor = baseColor * vec4(out_ambient + out_diffuse + out_specular, 1);
    } else {
        FragColor = baseColor;
    }
}
