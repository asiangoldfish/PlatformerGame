/**
 * All materials come from the following source:
 * <a href=http://devernay.free.fr/cours/opengl/materials.html>devernay.free.fr</a>
 */

#pragma once

#include "pch.h"

// External
#include <glm/glm.hpp>

// Framework
#include "Shader.h"
#include "TextureManager.h"
#include "Memory.h"
#include "Log.h"

namespace FW {
    /**
     * Predefined material presets.
     *
     * @details Available presets are listed in this enum. When <u>MaterialPreset::CUSTOM</u> is specified, the API
     * expects the user to pass custom parameters to the material.
     */
    enum class MaterialPreset
    {
        CUSTOM = 0,

        // Metals
        EMERALD,
        JADE,
        OBSIDIAN,
        PEARL,
        RUBY,
        TURQUOISE,
        BRASS,
        BRONZE,
        CHROME,
        COPPER,
        GOLD,
        SILVER,

        // Plastics
        BLACK_PLASTIC,
        CYAN_PLASTIC,
        GREEN_PLASTIC,
        RED_PLASTIC,
        WHITE_PLASTIC,
        YELLOW_PLASTIC,

        // Rubbers
        BLACK_RUBBER,
        CYAN_RUBBER,
        GREEN_RUBBER,
        RED_RUBBER,
        WHITE_RUBBER,
        YELLOW_RUBBER
    };

    /**
     * Properties that change the shaders' output.
     *
     * @details The material properties are values fed to shaders. These impact a geometry's visual appearance.
     */
    struct MaterialProperties
    {
        glm::vec3 ambient = glm::vec3(1.0f);
        glm::vec3 diffuse = glm::vec3(1.0f);
        glm::vec3 specular = glm::vec3(1.0f);
        std::string diffuseTextureName;
        std::string specularTextureName;
        uint32_t diffuseTextureID = -1;
        uint32_t specularTextureID = -1;
        float shininess = 1.0f;

        /**
         * Set the diffuse texture's ID by its name.
         * @details TextureManage is used to find the texture's ID. Simply pass the name, and the ID is extracted.
         * @param name The texture's name
         */
        void setDiffuseTextureID(std::string name) {
            uint32_t foundID = TextureManager::getTextureID(name);
            if (foundID == TextureManager::getInvalidTextureID()) {
                WARN("Material::setDiffuseTextureID: Cannot find texture ID by name");
            } else {
                diffuseTextureID = foundID;
            }
        }

        /**
         * Set the specular texture's ID by its name.
         * @details TextureManage is used to find the texture's ID. Simply pass the name, and the ID is extracted.
         * @param name The texture's name
         */
        void setSpecularTextureID(std::string name) {
            uint32_t foundID = TextureManager::getTextureID(name);
            if (foundID == TextureManager::getInvalidTextureID()) {
                WARN("Material::setSpecularTextureID: Cannot find texture ID by name");
            } else {
                specularTextureID = foundID;
            }
        }
    };

    /**
     * The material represents the physical properties that affects a geometry's appearance.
     *
     * @details The material mimics properties of physical objects. It should be applied to an individual geometry or
     * model, ideally with aggregation. Call <u>draw()</u> to upload properties to the shader.
     *
     * @details Please note that the shader passed to <u>draw()</u> must support the material properties before they
     * will have any effect. Uploading only a selected number of properties must be done manually. <u>draw()</u>
     * should therefore not be used in this case.
     */
    class Material
    {
    public:
        /**
         * Default constructor.
         *
         * @details The material can be initialized by simply passing a preset. Doing so will populate material
         * properties with the preconfigured values.
         *
         * @param preset Material preset to populate material properties with. Ust MaterialPreset::CUSTOM to use your
         * own values.
         */
        Material(const MaterialPreset& preset = MaterialPreset::CUSTOM);

        /**
         * Define the material's properties with a preset.
         *
         * @details Call this method to populate the material properties with a preset after initializing the Material
         * instance. This is useful for changing the preset in runtime.
         *
         * @param preset Preset to get material properties from.
         */
        void setMaterialPreset(const MaterialPreset& preset);

        /**
         * Return this material's properties.
         *
         * @details Properties can be accessed and changed with this method.
         *
         * @example
         * @code
         * Material mat;
         * mat.getProperties().shininess = 1.0f;
         * @endcode
         */
        MaterialProperties& getProperties() { return properties; }

        /**
         * Upload properties to the shader.
         *
         * @details Call this before drawing a geometry or model. This ensures that anything draw using the specified
         * shader will use this material's properties.
         *
         * @param shader The shader to upload properties to.
         */
        virtual void draw(const Shader& shader);

    private:
        /**
         * Update the material's properties based on the current preset.
         */
        void updateBasedOnPreset();

    private:
        MaterialPreset materialPreset = MaterialPreset::CUSTOM;
        MaterialProperties properties;
    };
} // Framework
