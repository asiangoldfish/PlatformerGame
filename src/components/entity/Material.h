/**
 * All materials come from the following source:
 * http://devernay.free.fr/cours/opengl/materials.html
 */

#ifndef PLATFORMERGAME_MATERIAL_H
#define PLATFORMERGAME_MATERIAL_H

#include <glm/glm.hpp>

namespace Rendering {

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
     * Properties available for the material.
     *
     * The properties are what changes the material's attributes and visual
     * result.
     */
    struct MaterialProperties
    {
        glm::vec3 ambient, diffuse, specular;
        float shininess;

        void setAmbient(const glm::vec3& value) { ambient = value; }
        void setDiffuse(const glm::vec3& value) { diffuse = value; }
        void setSpecular(const glm::vec3& value) { specular = value; }
        void setShininess(const float value) { shininess = value; }
    };

    class Material
    {
    public:
        Material(
          const MaterialPreset& preset = MaterialPreset::CUSTOM);

        /**
         * Define the material's properties with a preset.
         * @param preset Preset to get base the properties from.
         */
        void setMaterialPreset(const MaterialPreset& preset);

        MaterialProperties getProperties() const { return properties; }

    private:
        /**
         * Update the material's properties based on the current preset.
         */
        void updateBasedOnPreset();

    private:
        MaterialPreset materialPreset = MaterialPreset::CUSTOM;
        MaterialProperties properties;
    };

} // Rendering

#endif // PLATFORMERGAME_MATERIAL_H
