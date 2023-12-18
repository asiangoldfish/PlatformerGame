#version 430 core

in vec2 o_texCoord;
in vec3 o_position;
in vec4 o_color;
in vec4 o_fragPosition;
in vec3 o_normal;

out vec4 color;

layout (binding = 0) uniform sampler2D   u_textureSampler2D;
layout (binding = 1) uniform samplerCube u_textureSampler3D;
uniform int         u_enableTexture = 0;

uniform int u_samplerSwitch = 0;

// Albedo color multiplier. Albedo means base color here.
uniform vec4 u_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

// Phong
uniform vec3 u_lightSourcePosition = vec3(1.0f);  // Receives main:camera.getPosition()
uniform vec3 u_ambientColor        = vec3(1.0f);  // Receives main:ambientColorCycle
uniform vec3 u_diffuseColor        = vec3(1.0f);  // Receives main:glm::vec4(...)
uniform int u_enableLighting       = 1;

uniform vec3 u_cameraPosition = vec3(0.0f);  // For specular illumination
uniform vec3 u_specularColor  = vec3(1.0f);  // Receives main:glm::vec4(...)

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

    // Diffuse computations
    vec3 norm = normalize(o_normal);
    vec3 lightDirection = normalize((u_lightSourcePosition.xyz) - o_fragPosition.xyz);
    vec3 diffuseColor = max(dot(norm, lightDirection), 0.0f) * u_diffuseColor;

    // Specular computations
    vec3 reflectedLight    = normalize(reflect(-lightDirection, norm));
    vec3 observerDirection = normalize(u_cameraPosition - o_fragPosition.xyz);

    /*
     * Specular formula: (Source: issue 22 by Rafael Palomar)
     *
     * Where str is specFactor, ref is reflectedLight, obs is
     * observerDirection, n is arbitrary shininess factor, and "."
     * signifies dot product. Here split across 2 lines of code.
     *
     * This comment doesn't necessarily need to be here.
     * If it's not of convenience feel free to remove.
     *
     * S = str . (ref . obs)^n
     */
    float specFactor = pow(max(dot(observerDirection, reflectedLight), 0.0f), 12);
    vec3  specular = specFactor * u_specularColor;

    // Compute the final colors
    if (u_enableLighting == 1) {
        color = baseColor * vec4(u_ambientColor + diffuseColor + specular, 1); // Lighting added to the computation
    } else {
        color = baseColor;
    }
}
