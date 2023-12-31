#include "Material.h"
#include "TextureManager.h"

namespace FW {
    Material::Material(const MaterialPreset& preset)
      : materialPreset(preset)
    {
        updateBasedOnPreset();
    }

    void Material::updateBasedOnPreset()
    {
        // Only set attributes if not set to custom
        if (materialPreset != MaterialPreset::CUSTOM) {
            switch (materialPreset) {
                case MaterialPreset::EMERALD:
                    properties.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
                    properties.diffuse =
                      glm::vec3(0.07568f, 0.61424f, 0.07568f);
                    properties.specular = glm::vec3(0.633f, 0.727811f, 0.633f);
                    properties.shininess = 0.6f;
                    break;
                case MaterialPreset::JADE:
                    properties.ambient = glm::vec3(0.135f, 0.2225f, 0.1575f);
                    properties.diffuse = glm::vec3(0.54f, 0.89f, 0.63f);
                    properties.specular =
                      glm::vec3(0.316228f, 0.316228f, 0.316228f);
                    properties.shininess = 0.1f;
                    break;
                case MaterialPreset::OBSIDIAN:
                    properties.ambient = glm::vec3(0.05375f, 0.05f, 0.06625f);
                    properties.diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f);
                    properties.specular =
                      glm::vec3(0.332741f, 0.328634f, 0.346435f);
                    properties.shininess = 0.3f;
                    break;
                case MaterialPreset::PEARL:
                    properties.ambient = glm::vec3(0.25f, 0.20725f, 0.20725f);
                    properties.diffuse = glm::vec3(1.0f, 0.829f, 0.829f);
                    properties.specular =
                      glm::vec3(0.296648f, 0.296648f, 0.296648f);
                    properties.shininess = 0.088f;
                    break;
                case MaterialPreset::RUBY:
                    properties.ambient = glm::vec3(0.1745f, 0.01175f, 0.01175f);
                    properties.diffuse =
                      glm::vec3(0.61424f, 0.04136f, 0.04136f);
                    properties.specular =
                      glm::vec3(0.727811f, 0.626959f, 0.626959f);
                    properties.shininess = 0.6f;
                    break;
                case MaterialPreset::TURQUOISE:
                    properties.ambient = glm::vec3(0.1f, 0.18725f, 0.1745f);
                    properties.diffuse = glm::vec3(0.396f, 0.74151f, 0.69102f);
                    properties.specular =
                      glm::vec3(0.297254f, 0.30829f, 0.306678f);
                    properties.shininess = 0.1f;
                    break;
                case MaterialPreset::BRASS:
                    properties.ambient =
                      glm::vec3(0.329412f, 0.223529f, 0.027451f);
                    properties.diffuse =
                      glm::vec3(0.780392f, 0.568627f, 0.113725f);
                    properties.specular =
                      glm::vec3(0.992157f, 0.941176f, 0.807843f);
                    properties.shininess = 0.21794872f;
                    break;
                case MaterialPreset::BRONZE:
                    properties.ambient = glm::vec3(0.2125f, 0.1275f, 0.054f);
                    properties.diffuse = glm::vec3(0.714f, 0.4284f, 0.18144f);
                    properties.specular =
                      glm::vec3(0.393548f, 0.271906f, 0.166721f);
                    properties.shininess = 0.2f;
                    break;
                case MaterialPreset::CHROME:
                    properties.ambient = glm::vec3(0.25f, 0.25f, 0.25f);
                    properties.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
                    properties.specular =
                      glm::vec3(0.774597f, 0.774597f, 0.774597f);
                    properties.shininess = 0.6f;
                    break;
                case MaterialPreset::COPPER:
                    properties.ambient = glm::vec3(0.19125f, 0.0735f, 0.0225f);
                    properties.diffuse = glm::vec3(0.7038f, 0.27048f, 0.0828f);
                    properties.specular =
                      glm::vec3(0.256777f, 0.137622f, 0.086014f);
                    properties.shininess = 0.1f;
                    break;
                case MaterialPreset::GOLD:
                    properties.ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
                    properties.diffuse =
                      glm::vec3(0.75164f, 0.60648f, 0.22648f);
                    properties.specular =
                      glm::vec3(0.628281f, 0.555802f, 0.366065f);
                    properties.shininess = 0.4f;
                    break;
                case MaterialPreset::SILVER:
                    properties.ambient =
                      glm::vec3(0.19225f, 0.19225f, 0.19225f);
                    properties.diffuse =
                      glm::vec3(0.50754f, 0.50754f, 0.50754f);
                    properties.specular =
                      glm::vec3(0.508273f, 0.508273f, 0.508273f);
                    properties.shininess = 0.4f;
                    break;
                case MaterialPreset::BLACK_PLASTIC:
                    properties.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
                    properties.diffuse = glm::vec3(0.01f, 0.01f, 0.01f);
                    properties.specular = glm::vec3(0.50f, 0.50f, 0.50f);
                    properties.shininess = .25f;
                    break;
                case MaterialPreset::CYAN_PLASTIC:
                    properties.ambient = glm::vec3(0.0f, 0.1f, 0.06f);
                    properties.diffuse =
                      glm::vec3(0.0f, 0.50980392f, 0.50980392f);
                    properties.specular =
                      glm::vec3(0.50196078f, 0.50196078f, 0.50196078f);
                    properties.shininess = .25f;
                    break;
                case MaterialPreset::GREEN_PLASTIC:
                    properties.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
                    properties.diffuse = glm::vec3(0.1f, 0.35f, 0.1f);
                    properties.specular = glm::vec3(0.45f, 0.55f, 0.45f);
                    properties.shininess = .25f;
                    break;
                case MaterialPreset::RED_PLASTIC:
                    properties.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
                    properties.diffuse = glm::vec3(0.5f, 0.0f, 0.0f);
                    properties.specular = glm::vec3(0.7f, 0.6f, 0.6f);
                    properties.shininess = .25f;
                    break;
                case MaterialPreset::WHITE_PLASTIC:
                    properties.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
                    properties.diffuse = glm::vec3(0.55f, 0.55f, 0.55f);
                    properties.specular = glm::vec3(0.70f, 0.70f, 0.70f);
                    properties.shininess = .25f;
                    break;
                case MaterialPreset::YELLOW_PLASTIC:
                    properties.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
                    properties.diffuse = glm::vec3(0.5f, 0.5f, 0.0f);
                    properties.specular = glm::vec3(0.60f, 0.60f, 0.50f);
                    properties.shininess = .25f;
                    break;
                case MaterialPreset::BLACK_RUBBER:
                    properties.ambient = glm::vec3(0.02f, 0.02f, 0.02f);
                    properties.diffuse = glm::vec3(0.01f, 0.01f, 0.01f);
                    properties.specular = glm::vec3(0.4f, 0.4f, 0.4f);
                    properties.shininess = .078125f;
                    break;
                case MaterialPreset::CYAN_RUBBER:
                    properties.ambient = glm::vec3(0.0f, 0.05f, 0.05f);
                    properties.diffuse = glm::vec3(0.4f, 0.5f, 0.5f);
                    properties.specular = glm::vec3(0.04f, 0.7f, 0.7f);
                    properties.shininess = .078125f;
                    break;
                case MaterialPreset::GREEN_RUBBER:
                    properties.ambient = glm::vec3(0.0f, 0.05f, 0.0f);
                    properties.diffuse = glm::vec3(0.4f, 0.5f, 0.4f);
                    properties.specular = glm::vec3(0.04f, 0.7f, 0.04f);
                    properties.shininess = .078125f;
                    break;
                case MaterialPreset::RED_RUBBER:
                    properties.ambient = glm::vec3(0.05f, 0.0f, 0.0f);
                    properties.diffuse = glm::vec3(0.5f, 0.4f, 0.4f);
                    properties.specular = glm::vec3(0.7f, 0.04f, 0.04f);
                    properties.shininess = .078125f;
                    break;
                case MaterialPreset::WHITE_RUBBER:
                    properties.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
                    properties.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
                    properties.specular = glm::vec3(0.7f, 0.7f, 0.7f);
                    properties.shininess = .078125f;
                    break;
                case MaterialPreset::YELLOW_RUBBER:
                    properties.ambient = glm::vec3(0.05f, 0.05f, 0.0f);
                    properties.diffuse = glm::vec3(0.5f, 0.5f, 0.4f);
                    properties.specular = glm::vec3(0.7f, 0.7f, 0.04f);
                    properties.shininess = .078125f;
                    break;

                case MaterialPreset::CUSTOM:
                    properties.ambient = glm::vec3(0.2f);
                    properties.diffuse = glm::vec3(0.2f);
                    properties.specular = glm::vec3(0.2f);
                    properties.shininess = 1.0f;
            }
        }
    }

    void Material::setMaterialPreset(const MaterialPreset& preset)
    {
        materialPreset = preset;
        updateBasedOnPreset();
    }

    void Material::draw(const Shader& shader)
    {
        shader.setFloat3("u_material.ambient", properties.ambient);
        shader.setFloat3("u_material.diffuse", properties.diffuse);
        shader.setFloat3("u_material.specular", properties.specular);
        shader.setFloat("u_material.shininess", properties.shininess);

        TextureManager::bind(properties.diffuseTextureID, 0);
        TextureManager::bind(properties.specularTextureID, 1);
    }
} // Framework
