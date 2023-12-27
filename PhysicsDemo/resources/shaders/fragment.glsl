#version 430 core

in vec2 o_texCoord;
in vec3 o_position;
in vec4 o_color;
in vec4 o_fragPosition;
in vec3 o_normal;
in vec3 o_modelPosition;

out vec4 FragColor;

uniform int         u_enableTexture = 0;

uniform int u_samplerSwitch = 0;

// Albedo color multiplier. Albedo means base color here.
uniform vec4 u_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

// Phong
uniform bool u_enableLighting       = true;

uniform vec3 u_cameraPosition = vec3(0.0f);// For specular illumination

/** Determines how to render objects, like coloured or by depth */
uniform int u_visualizeMode = 0;

layout(location = 0) uniform samplerCube u_skybox;

/**
 * Material properties grouped into a single structure.
 */
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material u_material;

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_DIRECTIONAL_LIGHT 128
uniform DirectionalLight u_directionalLight[NR_DIRECTIONAL_LIGHT];
uniform int numOfDirectionalLights = 0;

struct PointLight {
    vec3 position;
    float brightness;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 128
uniform PointLight u_pointLight[NR_POINT_LIGHTS];
uniform int numOfPointLights = 0;

vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 computePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec4 renderColoredMode();
vec4 renderDepthTestingMode(float nearClip, float farClip);

void main() {
    switch (u_visualizeMode) {
        case 0: FragColor = renderColoredMode(); break;
        case 1: FragColor = renderDepthTestingMode(0.1, 100.0); break;
    }

}

vec4 renderColoredMode() {
    vec4 tex = vec4(1.0f);
    vec4 baseColor;

    tex = texture(u_material.diffuse, o_texCoord).rgba;
    baseColor = mix(tex, o_color * u_color, 0.4);

    // Lighting
    vec3 normal = normalize(o_normal);
    vec3 viewDir = normalize(u_cameraPosition - o_fragPosition.xyz);
    vec3 result = vec3(0.0);

    // Phase 1: Directional lighting
    for (int i = 0; i < numOfDirectionalLights; i++) {
        result += computeDirectionalLight(u_directionalLight[i], normal, viewDir);
    }

    // Phase 2: Point lights
    for (int i = 0; i < numOfPointLights; i++) {
        result += computePointLight(u_pointLight[i], normal, o_fragPosition.xyz, viewDir);
    }

    // Light from cubemap
    vec3 I = normalize(o_modelPosition - u_cameraPosition);
    vec3 R = reflect(I, normal);

    return baseColor; // * vec4(result, 1.0); //vec4(texture(u_skybox, R).rgb, 1.0);
}

vec4 renderDepthTestingMode(float nearClip, float farClip) {
    float depth = gl_FragCoord.z / farClip;
    float ndc = depth * 2.0 - 1.0;
    float linearDepth =
        (2.0 * nearClip * farClip) / (farClip + nearClip - ndc * (farClip - nearClip));

    return vec4(vec3(linearDepth), 1.0);
}

// FUNCTION: Directional light
vec3 computeDirectionalLight(DirectionalLight dirlight, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-dirlight.direction);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

    // Combine results
    vec3 ambient = dirlight.ambient * vec3(texture(u_material.diffuse, o_texCoord));
    vec3 diffuse = dirlight.diffuse * diff * vec3(texture(u_material.diffuse, o_texCoord));
    vec3 specular = dirlight.specular * spec * vec3(texture(u_material.specular, o_texCoord));

    return ambient + diffuse + specular;
}

// FUNCTION: Point light
vec3 computePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * pow(distance, 2));

    // Combine results
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, o_texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, o_texCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, o_texCoord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * light.brightness;
}
